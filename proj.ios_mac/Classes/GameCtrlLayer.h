#pragma once
#include"cocos2d.h"
#include"Snake.h"
#include"Food.h"
USING_NS_CC;
namespace SnakeSpace{
	class GameCtrlLayer;
	class GameCtrlLayer_Classic;
	class GameCtrlLayer_Creazy;
	class HelpLayer;

	class GameCtrlLayer :public Layer{
	public:
		GameCtrlLayer();
		virtual ~GameCtrlLayer();
	public:
		SnakeCtrl* getSnakeCtrlLayer()const{ return m_pSnakeCtrlLayer; };
		void setSnakeCtrlLayer(SnakeCtrl* scl) { m_pSnakeCtrlLayer = scl; };
		FoodCtrl* getFoodCtrlLayer() const { return m_pFoodCtrlLayer; };
		void setFoodCtrlLayer(FoodCtrl* fcl)  { m_pFoodCtrlLayer = fcl; };
	public:
		bool onTouchBeganCallBack(Touch* t, Event* e);
	protected:
        int m_modeTag;  //保存当前的模式信息
		Size m_visbleSize;
		Point m_orginPos;
		Sprite* m_pBgSprite;
        
		EventListenerTouchOneByOne* m_pEvenTListener;
		LabelBMFont* m_pStartLabel;

		void collision();
		bool checkUpBouding();
		bool checkDownBounding();
		bool checkRightBounding();
		bool checkLeftBounding();

		virtual void judgeCollision(SnakeSpace::SnakeNode* s, SnakeSpace::Food* f);
		virtual void checkBouding();
		virtual void judgeVictory();
		virtual void judgeOver();

		
	private:
		SnakeCtrl*  m_pSnakeCtrlLayer;
		FoodCtrl* m_pFoodCtrlLayer;
	

	};


	
	class GameCtrlLayer_Classic :public GameCtrlLayer{
	public:
		GameCtrlLayer_Classic();
		virtual ~GameCtrlLayer_Classic();
	public:
		virtual bool init();
		virtual void update(float dt);
		virtual void onEnter();
		virtual void onExit();
		CREATE_FUNC(GameCtrlLayer_Classic);
	private:
		LabelBMFont* m_pScoreLable;
		LabelBMFont* m_pHealthLable;

	};

	class GameCtrlLayer_Creazy :public GameCtrlLayer{
	public:
		GameCtrlLayer_Creazy();
		virtual ~GameCtrlLayer_Creazy();
	public:
		virtual bool init();
		virtual void update(float dt);
		virtual void onEnter();
		virtual void onExit();
		CREATE_FUNC(GameCtrlLayer_Creazy);
	private:
		LabelBMFont* m_pHealthLable;
		LabelBMFont* m_pScoreLable;

//		virtual void judgeCollision(SnakeSpace::SnakeNode* s, SnakeSpace::Food* f);
	};


	class HelpLayer:public Layer{
	public:
		HelpLayer();
		virtual ~HelpLayer();
		virtual bool init();
		CREATE_FUNC(HelpLayer);
	private:
		Label* m_pBack;
	};

}
