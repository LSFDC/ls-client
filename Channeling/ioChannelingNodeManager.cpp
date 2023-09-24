#include "stdafx.h"
#include "iochannelingnodeWeMadeBuy.h"
#include "iochannelingnodemgame.h"
#include ".\iochannelingnodemanager.h"
#include "iochannelingnodedaum.h"
#include "ioChannelingNodeNaver.h"
#include "ioChannelingNodeTooniland.h"
#include "ioChannelingNodeNexon.h"
#include "ioChannelingNodeHanGame.h"

#ifdef STEAM_ATTACH
#include "ioChannelingNodeSteam.h"
#endif

template<> ioChannelingNodeManager* Singleton< ioChannelingNodeManager >::ms_Singleton = 0;

ioChannelingNodeManager::ioChannelingNodeManager(void)
{
	m_vChannelingNodeVector.reserve( 10 );
}

ioChannelingNodeManager::~ioChannelingNodeManager(void)
{
	for(vChannelingNodeVector::iterator iter = m_vChannelingNodeVector.begin(); iter != m_vChannelingNodeVector.end(); ++iter)
	{
	    delete *iter;
	}
	m_vChannelingNodeVector.clear();
}

ioChannelingNodeManager & ioChannelingNodeManager::GetSingleton()
{
	return Singleton< ioChannelingNodeManager >::GetSingleton();
}

void ioChannelingNodeManager::Init()
{
	AddNode( CreateNode( CNT_WEMADEBUY ) );
#ifdef STEAM_ATTACH
	AddNode( CreateNode( CNT_STEAM ));
#endif
	AddNode( CreateNode( CNT_MGAME ) );
	AddNode( CreateNode( CNT_DAUM ) );
	AddNode( CreateNode( CNT_NAVER ) );
	AddNode( CreateNode( CNT_TOONILAND ) );
	AddNode( CreateNode( CNT_NEXON ) );
	AddNode( CreateNode( CNT_HANGAME ) );
	// ���ο� ä�θ� �߰�
}

ioChannelingNodeParent * ioChannelingNodeManager::CreateNode( ChannelingType eChannelingType )
{
	ioChannelingNodeParent *pNode = NULL;
	if( eChannelingType == CNT_WEMADEBUY )
		pNode = new ioChannelingNodeWemadeBuy;
#ifdef STEAM_ATTACH
	else if( eChannelingType == CNT_STEAM )
		pNode = new ioChannelingNodeSteam;
#endif
	else if( eChannelingType == CNT_MGAME )
		pNode = new ioChannelingNodeMgame;
	else if( eChannelingType == CNT_DAUM )
		pNode = new ioChannelingNodeDaum;
	else if( eChannelingType == CNT_NAVER )
		pNode = new ioChannelingNodeNaver;
	else if( eChannelingType == CNT_TOONILAND )
		pNode = new ioChannelingNodeTooniland;
	else if( eChannelingType == CNT_NEXON )
		pNode = new ioChannelingNodeNexon;
	else if ( eChannelingType == CNT_HANGAME )
		pNode = new ioChannelingNodeHanGame;
	// ���ο� ä�θ� �߰�
	else
		LOG.PrintTimeAndLog( 0, "%s Type is wrong. %d", __FUNCTION__, (int) eChannelingType );

	return pNode;
}

void ioChannelingNodeManager::AddNode( ioChannelingNodeParent *pNode )
{
	if( !pNode )
	{
		LOG.PrintTimeAndLog( 0, "%s pNode == NULL." , __FUNCTION__ );
		return;
	}

	if( GetNode( pNode->GetType() ) )
	{
		LOG.PrintTimeAndLog( 0, "%s Type is existing.%d" , __FUNCTION__ , pNode->GetType() );
		return;
	}

	m_vChannelingNodeVector.push_back( pNode );
}

ioChannelingNodeParent * ioChannelingNodeManager::GetNode( ChannelingType eChannelingType )
{
	for(vChannelingNodeVector::iterator iter = m_vChannelingNodeVector.begin(); iter != m_vChannelingNodeVector.end(); ++iter)
	{
	    ioChannelingNodeParent *pNode = *iter;
		if( !pNode )
			continue;
		if( pNode->GetType() == eChannelingType )
			return pNode;
	}

	return NULL;
}