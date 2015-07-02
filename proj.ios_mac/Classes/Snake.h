#pragma once
#include"cocos2d.h"

 enum Direction{
	Up = 0,
	Right,
	Left,
	Down,
};

USING_NS_CC;
namespace SnakeSpace
{
	class SnakeNode;
	class Snake;
	class SnakeCtrl;


   //snake node 
	class SnakeNode:public Node{
	public:
		SnakeNode();
		~SnakeNode();
	public:
		static SnakeNode* createWithArgs(const std::string& nodeVal,const int tag,const Vec2& pos);
		bool  initWithArgs(const std::string& nodeVal,const int tag,const Vec2& pos);
		Sprite* getNodeSprite(){ return m_pSprite; };
	public:
		//body move
		void move(const Direction& dir,const Vec2& prePos,const float nodeDis);
		//head move
		void move(const Direction& dir, const float speed,const float dt);
		//change NodeVal;
		void changeNodeVal();
		//remova snake node
		void removeNode();
	public:
		const std::string& getNodeVal() const { return m_val; };
	private:
		Sprite* m_pSprite;
		 std::string m_val;
	};

	//snake 
	class Snake{
	public:
		Snake();
		~Snake();
		// get snake nodes
		static Vector<SnakeNode *>* getSnake(){ return m_pSnake; };
	    static void removeALLNodes();
	private:
		static Vector<SnakeNode *>* m_pSnake;
	};

	//snake ctrl
	class SnakeCtrl:public Layer{
	public:
		SnakeCtrl();
		~SnakeCtrl();
		CREATE_FUNC(SnakeCtrl);
		virtual bool init();
	public:
		virtual void onTouchEndedCallBack(Touch* t, Event* e);
		virtual void update(float dt);
		virtual void onEnter();
		virtual void onExit();
	public:
		EventListenerTouchOneByOne* getEventListener(){ return m_pEventListener; };
	public:
		// snake move dir
		void setCurDir(const Direction& dir) { m_curDir = dir; };

		// snake node combine
		void combineSnakeNode();

		//add Snake Node
		void addSnakeNode(const std::string& nodeVal);

		//reverse snake node;
		void reverseSnakeNodes();

		//delete a node
		void deleteSnakeNode(SnakeNode* s);
	private:

		Direction m_curDir;
        float m_speed;
		float m_nodeDis;

		Vec2 m_newNodeDisVec2[4];
		Size m_visbleSize;
		Point m_visbleOrginPos;
		
		EventListenerTouchOneByOne* m_pEventListener;
		//snkae move
 		void snakeMove(float dt);

	    //set Snake Tags
		void setSnakeTags();
        
	
	};

}
