#include "DG2014_Ver3_51.h"
#include "../myLib.h"

#pragma warning(disable:4316)
//#include "atlstr.h"

namespace DG_
{
	//-------------------------------------------------------------------
	Mesh::Mesh( )
	{
	}
	Mesh::~Mesh( )
	{
	}

	//-------------------------------------------------------------------
	//	メッシュを生成する
	bool Mesh::Initialize(	Material&				mc_,
							VB_Container::SP&		vbc_,//バーテックスバッファコンテナ
							Texture::SP&			tex_)//テクスチャ			
	{
		this->normalEnable  =  false;
		this->uvEnable  =  false;

		mateConts.push_back(mc_);

		MeshGroup  w;
		w.index  =  nullptr;
		w.mcNumber  =  0;
		meshGroups.push_back(w);
		vbc  =  vbc_;
		//BB・BS生成
		this->InitializeBB_BS( );
		return  true;
	}
	//-------------------------------------------------------------------
	//SOBファイルからメッシュを生成する
	Mesh::SP  Mesh::Create_FromSOBFile(	const string&			fpath_)
	{
		//実際の読み込みは別ファイルで実装
		Mesh::SP  rtv  =  Mesh::Create_FromSOBFileSub(fpath_);
		if(rtv){
			//BB・BS生成
			rtv->InitializeBB_BS( );
		}
		return rtv;
	}
	//-------------------------------------------------------------------
	//ＢＢとＢＳを生成する
	void  Mesh::InitializeBB_BS( )
	{
		//バウンディングボックス・スフィアを生成する
		auto  vb = this->vbc->GetVB(0);
		ML::Vec3  *p = (ML::Vec3*)vb->memory;
		//BB
		//最小・最大点の検出
		ML::Vec3  minP(p[0]), maxP(p[0]);
		for (UINT n = 1; n < vb->num; n++){
			minP.x = min(minP.x, p[n].x);
			minP.y = min(minP.y, p[n].y);
			minP.z = min(minP.z, p[n].z);

			maxP.x = max(maxP.x, p[n].x);
			maxP.y = max(maxP.y, p[n].y);
			maxP.z = max(maxP.z, p[n].z);
		}
		this->bb.Center = XMFLOAT3((minP + maxP) / 2);
		this->bb.Extents = XMFLOAT3((maxP - minP) / 2);
		//BS
		//もっとも遠い点を探す
		ML::Vec3  cen = (minP + maxP) / 2;	//球の中心（BBと同じとする）
		float  len = 0;
		for (UINT n = 1; n < vb->num; n++){
			ML::Vec3  v = p[n] - cen;
			float  l = v.Length();
			len = max(l, len);
		}
		this->bs.Center = XMFLOAT3(cen);
		this->bs.Radius = len;
	}


	//-------------------------------------------------------------------
	//プログラム上でメッシュを作る（普通は使わない）
	Mesh::SP  Mesh::Create(	Material&				mc_,
							VB_Container::SP&		vbc_,//バーテックスバッファコンテナ
							Texture::SP&			tex_)//テクスチャ	
	{
		if( Mesh::SP  sp  =  Mesh::SP( new  Mesh( ) ) )
		{
			if( sp->Initialize(mc_,  vbc_,  tex_) ){
				return sp;
			}
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	//	メッシュを表示する
	void Mesh::Draw( )
	{
		auto  dgi = DG_::DGObject::GetInst( );	if(dgi == 0){	return;}

		//テクニック選択？
		dgi->ETechnique( ).GetPassByIndex( 0 )->Apply( 0 );
		//頂点インプットレイアウトをセット
		this->inputLayout->IASet( );


		for(	auto  it = meshGroups.begin( );
				it != meshGroups.end( );	it++){
			auto&  mt = mateConts[it->mcNumber];
			if(mt.tex){
				//テクスチャの登録
				mt.tex->PSSet(0);
			}else{
			//	//テクスチャを使用しない
				Texture::Empty_PSSet(0);
			}
			//メッシュ定数情報を更新
			dgi->EState( ).param.mateDiffuse   =  mt.diffuse;
			dgi->EState( ).param.mateSpeculer  =  mt.speculer;
			dgi->EState( ).param.mateAmbient   =  mt.ambient;
			dgi->EState( ).param.mateEmissive  =  mt.emissive;

			//プリミティブ・トポロジーとバーテックスバッファーをセット
			vbc->IASet(0);
			//プリミティブをレンダリング
			//D3D10_TECHNIQUE_DESC  dc;
			//dgi->ETechnique( ).GetDesc( &dc );
			//for( UINT p = 0; p < dc.Passes; ++p )
			//{
			dgi->EState( ).Update_Params( );
				dgi->ETechnique( ).GetPassByIndex( 0 )->Apply( 0 );
				if(it->index){
					dgi->Device( ).IASetIndexBuffer(it->index->buf, DXGI_FORMAT_R32_UINT, 0 );
					dgi->Device( ).DrawIndexed( it->index->num, 0 , 0 );
				}
				else{
					dgi->Device( ).Draw( vbc->Num( ), 0 );
				}
			//}
		}
	}
	//-------------------------------------------------------------------
	//	メッシュとレイの接触判定を行う（レイはメッシュのローカル座標系）
	bool  Mesh::Hit_Ray(const  ML::Vec3&  pos_,  const  ML::Vec3&  dir_,  float&  dist_,  bool  qmode_)
	{
		bool  rtv = false;
		//バーテックスコンテナから頂点座標のバッファを得る
		VertexBuffer::SP  vbPos = this->vbc->GetVB(0);

		//メッシュは内部で分割されている可能性があるのでループで処理
		for (auto  it = meshGroups.begin();
			it != meshGroups.end();	it++){

			if (it->index){
				//インデックスあり（複数回の可能性あり）
				rtv = ML::Collsion::HitRayTriangle(pos_, dir_, qmode_, vbPos, 0,
					it->index, dist_);
				if (rtv  &&  qmode_){ break; }
			}
			else{
				//インデックスなし（たぶん１回）
				rtv = ML::Collsion::HitRayTriangle(pos_, dir_, qmode_, vbPos, 0, dist_);
				if (rtv  &&  qmode_){ break; }
			}
		}
		return  rtv;
	}
	//-------------------------------------------------------------------
	//	バウンディングボックスとレイの接触判定を行う（レイはメッシュのローカル座標系）
	bool  Mesh::Hit_RayBB(const  ML::Vec3&  pos_, const  ML::Vec3&  dir_, float&  dist_)
	{
		float  len = dir_.Length();
		const  FXMVECTOR  p = XMLoadFloat3(&XMFLOAT3(pos_));
		const  FXMVECTOR  d = XMLoadFloat3(&XMFLOAT3(dir_ / len));

		if (XNA::IntersectRayAxisAlignedBox(p, d, &this->bb, &dist_)){
			dist_ /= len;
			return true;
		}
		return  false;
	}

}
