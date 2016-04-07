#pragma once
#include"cocos2d.h"
USING_NS_CC;



namespace SnakeSpace{
	class Food;
	class Foods;
	class FoodCtrl;
	class FoodCreatePos;

	class Food :public Node{
	public:
		Food();
		virtual ~Food();
		static Food* createWithArgs(const std::string& fVal, const int tag, FoodCreatePos* createPos, const float appearTime);
		bool initWithArgs(const std::string& fVal, const int tag, FoodCreatePos* createPos, const float appearTime);
	public:
		virtual void update(float dt);
	public:
		//remove Food;
		void removeFood();
		Sprite* getFoodSprite() const { return m_pSprite; };

		void setShouldDisappear(bool should){ m_shouldDisappear = should; };
		bool getShouldDisappear() const { return m_shouldDisappear; };

		void setCanEat(bool canEat){ m_canEat = canEat; };
		bool getCanEat() const {return m_canEat; };
	public:
		const std::string& getFoodVal() const { return m_val; }; 
		void changeFood(const std::string& val);
	private:
		std::string  m_val;
		Sprite* m_pSprite;
		FoodCreatePos* m_pos;

		float m_curTime;
		float m_appearenctTime;


		bool m_canEat;
		bool m_shouldDisappear;
	
		
	};


	class Foods{
	public:
		Foods();
		~Foods();
	public:
		static  Vector<Food *>* getFoods(){ return m_pFoods; };
		static void removeAllFoods();
	private:
		static Vector<Food *>* m_pFoods;
	};

    
	class FoodCreatePos :public Node
	{
	public:
		FoodCreatePos();
		~FoodCreatePos();
		bool initWithArgs(const Vec2& pos);
		static FoodCreatePos* createWithArgs(const Vec2& pos);
	public:
		const Vec2& getPos() const { return m_createPos; };

		void setIsCreated(bool created){ m_isCreate = created; };
	    const  bool getIsCreated() const { return m_isCreate; };
	private:
		Vec2 m_createPos;
		bool  m_isCreate;

	};


	class FoodCtrl:public Layer{
	public:
		FoodCtrl();
		virtual ~FoodCtrl();
	public:
		//create a food	private:
		void createFood(const std::string& fVal);

		//rand foodVal
		virtual const std::string& getRandFoodVal();
	protected:
		Vector<FoodCreatePos*>* m_pFoodCreatePos;
		std::vector<std::string *>* m_pFoodVals;

		//init create food's position
		virtual void initCreatePos();

		//init food's vals
		virtual void initFoodVals();

		//update current food
		virtual void updateFoods();

	};

	class FoodCtrl_Classic :public FoodCtrl
	{
	public:
		FoodCtrl_Classic();
		virtual ~FoodCtrl_Classic();
		virtual bool init();
		CREATE_FUNC(FoodCtrl_Classic);
	public:
		virtual void update(float dt);
		virtual void onEnter();
		virtual void onExit();
	private:
		float m_curTime;
	};
	
	class FoodCtrl_Creazy :public FoodCtrl
	{
	public:
		FoodCtrl_Creazy();
		virtual ~FoodCtrl_Creazy();
		virtual bool init();
		CREATE_FUNC(FoodCtrl_Creazy);
	public:
		virtual void update(float dt);
		virtual void onEnter();
		virtual void onExit();
	private:
		virtual void updateFoods();
		float m_curTime;
	};
	
}