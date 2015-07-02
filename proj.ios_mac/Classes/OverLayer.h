#pragma once
#include"cocos2d.h"
USING_NS_CC;
namespace SnakeSpace
{
	class OverLayerBase;

	class OverLayerBase :public Layer
	{
	public:
		OverLayerBase();
		virtual ~OverLayerBase();
	public:
		virtual bool init();
        virtual void onEnter();
        virtual void onExit();
		bool onTouchCallBack(Touch *t, Event* e);
		CREATE_FUNC(OverLayerBase);
	private:
		Size m_visbleSize;
		Point m_visbleOrgin;
		LabelBMFont* m_pModeLabel;
		LabelBMFont* m_pSuccessOrFail;
		LabelBMFont* m_pBestScore;
		LabelBMFont* m_pCureScore;
		Label* m_pReStart;
		Label* m_pBack;

		void setLabelPos();
	};
}