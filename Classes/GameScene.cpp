#include "GameScene.h"


USING_NS_CC;



Scene* GameScene::createScene()
{
    return GameScene::create();
}


bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
	Global::instance()->gameScene = this;

	//创建GameLayer 作为GameMap的框架
	GameLayer* gamelayer = GameLayer::createGameLayer(0);
	this->addChild(gamelayer,-1);
	gamelayer->setPosition(192 , TIPBARHEIGHT);
	//创建左边的状态栏
	Sprite* statusLayer = Sprite::create("ControlImage.png",Rect(0,0,311,580));
	statusLayer->setScale(0.75f);
	statusLayer->setAnchorPoint(Vec2::ZERO);
	this->addChild(statusLayer, -1);
	statusLayer->setPosition(-40, TIPBARHEIGHT);
	//创建最下面的信息栏
	LayerColor* downTipBar = LayerColor::create(Color4B::WHITE, STATUSBARWIDTH + MAP_SIZE, TIPBARHEIGHT);
	this->addChild(downTipBar, -1);
	downTipBar->setPosition(0, 0);
	//初始化信息栏
	TTFConfig ttfconfig1("fonts/arial.ttf", 12);
	auto tipBarLabel = Label::createWithTTF(ttfconfig1, NORMAL_TIPBAR); //初始化TipBar
	this->addChild(tipBarLabel, kZTipBar, kZTipBar);
	tipBarLabel->setAnchorPoint(Vec2::ZERO);
	tipBarLabel->setPosition(5, 5);
	tipBarLabel->setColor(Color3B::BLACK);
	//创建控制层
	ControlLayer* controllayer = ControlLayer::create();
	this->addChild(controllayer, 2);
	controllayer->setPosition(Vec2::ZERO);


	//创建左侧显示人物信息的状态条
	ttfconfigStatusBar = TTFConfig("fonts/arial.ttf", 14);
	ttfconfigStatusBar.bold = true;
	//显示当前楼层
	std::string temp = std::to_string(Global::instance()->currentLevel);
	auto currentlevel = Label::createWithTTF(ttfconfigStatusBar, "current level : " + temp);
	currentlevel->setAnchorPoint(Vec2::ZERO);
	currentlevel->setColor(Color3B::WHITE);
	currentlevel->setPosition(192 / 4.5, 32 * 12);
	this->addChild(currentlevel, kZcurrentlevel, kZcurrentlevel);
	//显示血量
	temp = std::to_string(Global::instance()->hero->HP);
	auto currentHP = Label::createWithTTF(ttfconfigStatusBar, "Hero HP : " + temp);
	currentHP->setAnchorPoint(Vec2::ZERO);
	currentHP->setColor(Color3B::WHITE);
	currentHP->setPosition(192 / 4.5, 32 * 11);
	this->addChild(currentHP, kZHP, kZHP);
	//显示攻击力
	temp = std::to_string(Global::instance()->hero->ATK);
	auto currentATK = Label::createWithTTF(ttfconfigStatusBar, "Hero ATK : " + temp);
	currentATK->setAnchorPoint(Vec2::ZERO);
	currentATK->setColor(Color3B::WHITE);
	currentATK->setPosition(192 / 4.5, 32 * 10);
	this->addChild(currentATK, kZATK, kZATK);
	//显示防御力
	temp = std::to_string(Global::instance()->hero->DEF);
	auto currentDEF = Label::createWithTTF(ttfconfigStatusBar, "Hero DEF : " + temp);
	currentDEF->setAnchorPoint(Vec2::ZERO);
	currentDEF->setColor(Color3B::WHITE);
	currentDEF->setPosition(192 / 4.5, 32 * 9);
	this->addChild(currentDEF, kZDEF, kZDEF);
	//显示黄钥匙数量
	temp = std::to_string(Global::instance()->hero->YellowKeys);
	auto currentYellowKeys = Label::createWithTTF(ttfconfigStatusBar, "Hero YellowKeys : " + temp);
	currentYellowKeys->setAnchorPoint(Vec2::ZERO);
	currentYellowKeys->setColor(Color3B::WHITE);
	currentYellowKeys->setPosition(192 / 4.5, 32 * 8);
	this->addChild(currentYellowKeys, kZYellowKeys, kZYellowKeys);
	//显示蓝钥匙数量
	temp = std::to_string(Global::instance()->hero->BlueKeys);
	auto currentBlueKeys = Label::createWithTTF(ttfconfigStatusBar, "Hero BlueKeys : " + temp);
	currentBlueKeys->setAnchorPoint(Vec2::ZERO);
	currentBlueKeys->setColor(Color3B::WHITE);
	currentBlueKeys->setPosition(192 / 4.5, 32 * 7);
	this->addChild(currentBlueKeys, kZBlueKeys, kZBlueKeys);
	//显示红钥匙数量
	temp = std::to_string(Global::instance()->hero->RedKeys);
	auto currentRedKeys = Label::createWithTTF(ttfconfigStatusBar, "Hero RedKeys : " + temp);
	currentRedKeys->setAnchorPoint(Vec2::ZERO);
	currentRedKeys->setColor(Color3B::WHITE);
	currentRedKeys->setPosition(192 / 4.5, 32 * 6);
	this->addChild(currentRedKeys, kZRedKeys, kZRedKeys);

	

}

//刷新左侧状态条 形参为kZoder的枚举常量 表示人物变化的信息类别
void GameScene::refreshStatus(kZorder order)
{
	Label* newLabel;
	GameScene* tempGameScene = Global::instance()->gameScene;
	if (order == kZcurrentlevel) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZcurrentlevel)));
		this->removeChildByTag(kZcurrentlevel);
		this->addChild(newLabel, kZcurrentlevel, kZcurrentlevel);
	}
	else if (order == kZHP) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZHP)));
		this->removeChildByTag(kZHP);
		this->addChild(newLabel, kZHP, kZHP);
	}
	else if (order == kZATK) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZATK)));
		this->removeChildByTag(kZATK);
		this->addChild(newLabel, kZATK, kZATK);
	}
	else if (order == kZDEF) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZDEF)));
		this->removeChildByTag(kZDEF);
		this->addChild(newLabel, kZDEF, kZDEF);
	}
	else if (order == kZYellowKeys) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZYellowKeys)));
		this->removeChildByTag(kZYellowKeys);
		this->addChild(newLabel, kZYellowKeys, kZYellowKeys);
	}
	else if (order == kZBlueKeys) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZBlueKeys)));
		this->removeChildByTag(kZBlueKeys);
		this->addChild(newLabel, kZBlueKeys, kZBlueKeys);
	}
	else if (order == kZRedKeys) {
		newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZRedKeys)));
		this->removeChildByTag(kZRedKeys);
		this->addChild(newLabel, kZRedKeys, kZRedKeys);
	}

	
}

void GameScene::refreshAllStatus()
{
	Label* newLabel;
	GameScene* tempGameScene = Global::instance()->gameScene;
	newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZcurrentlevel)));
	this->removeChildByTag(kZcurrentlevel);
	this->addChild(newLabel, kZcurrentlevel, kZcurrentlevel);
	newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZHP)));
	this->removeChildByTag(kZHP);
	this->addChild(newLabel, kZHP, kZHP);
	newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZATK)));
	this->removeChildByTag(kZATK);
	this->addChild(newLabel, kZATK, kZATK);
	newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZDEF)));
	this->removeChildByTag(kZDEF);
	this->addChild(newLabel, kZDEF, kZDEF);
	newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZYellowKeys)));
	this->removeChildByTag(kZYellowKeys);
	this->addChild(newLabel, kZYellowKeys, kZYellowKeys);
	newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZBlueKeys)));
	this->removeChildByTag(kZBlueKeys);
	this->addChild(newLabel, kZBlueKeys, kZBlueKeys);
	newLabel = createNewLabelForStatus((Label*)(tempGameScene->getChildByTag(kZRedKeys)));
	this->removeChildByTag(kZRedKeys);
	this->addChild(newLabel, kZRedKeys, kZRedKeys);
}

//创建状态栏的新状态条
Label* GameScene::createNewLabelForStatus(Label* oldLabel) {
	std::string newString;
	switch (oldLabel->getZOrder()) {
	case kZcurrentlevel:newString = std::string("current level : " + std::to_string(Global::instance()->currentLevel)); break;
	case kZHP:newString = std::string("Hero HP : " + std::to_string(Global::instance()->hero->HP)); break;
	case kZATK:newString = std::string("Hero ATK : " + std::to_string(Global::instance()->hero->ATK)); break;
	case kZDEF:newString = std::string("Hero DEF : " + std::to_string(Global::instance()->hero->DEF)); break;
	case kZYellowKeys:newString = std::string("Hero YellowKeys : " + std::to_string(Global::instance()->hero->YellowKeys)); break;
	case kZBlueKeys:newString = std::string("Hero BlueKeys : " + std::to_string(Global::instance()->hero->BlueKeys)); break;
	case kZRedKeys:newString = std::string("Hero RedKeys : " + std::to_string(Global::instance()->hero->RedKeys)); break;
	default:;
	}
	Label* newLabel = Label::createWithTTF(ttfconfigStatusBar, newString);
	newLabel->setAnchorPoint(Vec2::ZERO);
	newLabel->setColor(oldLabel->getColor());
	newLabel->setPosition(oldLabel->getPosition());
	return newLabel;

}

void GameScene::showTipBarText(std::string str)
{
	Label* label = (Label*)(this->getChildByTag(kZTipBar));
	TTFConfig ttfconfig("fonts/arial.ttf", 12);
	Label* newLabel = Label::createWithTTF(ttfconfig, str);
	newLabel->setAnchorPoint(Vec2::ZERO);
	newLabel->setPosition(label->getPosition());
	newLabel->setColor(label->getColor());
	this->removeChildByTag(kZTipBar);
	this->addChild(newLabel, kZTipBar, kZTipBar);
}

void GameScene::showSaveLayer()
{
	if (isShowingSaveLayer) { return; };
	
	isShowingSaveLayer = true;
	auto Saver = SaveControl::instance();
	

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto SaverLayer = LayerColor::create(Color4B::ORANGE);

	auto exit = MenuItemImage::create("Close/CloseNormal.png", "Close/CloseSelected.png", CC_CALLBACK_1(GameScene::SaverLayerExitButtonCallBack, this));
	auto exitButton = Menu::create(exit, NULL);
	exitButton->setPosition(exit->getContentSize().width / 2, MAP_SIZE);
	SaverLayer->addChild(exitButton, 0);

	this->addChild(SaverLayer, kZSaveLayer, kZSaveLayer);

	for (int i = 0; i <= 4; i++) { //五个存档子图层的位置
	
		auto subLayer = LayerColor::create(Color4B::BLUE, visibleSize.width * 3 / 5, visibleSize.height * 4 / 7 / 5);
		
		subLayer->setPosition((SaverLayer->getContentSize().width - 1 * subLayer->getContentSize().width) / 4 , (2 * i)*SaverLayer->getContentSize().height / 10 + 18);
		SaverLayer->addChild(subLayer, kZSaveLayer + 1, i);
		subLayer->setScale(1);
		if (Saver->checkIfTheSaveExisted(i)) {
			TTFConfig ttfconfig("fonts/arial.ttf", 18);
			ttfconfig.bold = true;
			Label* label = Label::createWithTTF(ttfconfig, createTheInfoShowedOnSaveLayer(i));
			label->setPosition(subLayer->getContentSize().width / 2, subLayer->getContentSize().height / 2);
			subLayer->addChild(label, kZSaveLayer + 1, i);
		}
		else {
			TTFConfig ttfconfig("fonts/arial.ttf", 18);
			ttfconfig.bold = true;
			Label* label = Label::createWithTTF(ttfconfig, "No Save Existed");
			label->setPosition(subLayer->getContentSize().width / 2, subLayer->getContentSize().height / 2);
			subLayer->addChild(label, kZSaveLayer + 1 , i);
		}
	}

	for (int i = 0; i < 5; i++) { //创建 保存 删除 按钮
		TTFConfig ttfconfig("fonts/arial.ttf", 18);
		ttfconfig.bold = true;
		auto menuSave = MenuItemFont::create("Save", [=](Ref* ref){
			SaveControl::instance()->save(i);
			auto subLayer = static_cast<LayerColor*>(SaverLayer->getChildByTag(i));
			subLayer->removeChildByTag(i);
			Label* label = Label::createWithTTF(ttfconfig, createTheInfoShowedOnSaveLayer(i));
			label->setPosition(subLayer->getContentSize().width / 2, subLayer->getContentSize().height / 2);
			subLayer->addChild(label, kZSaveLayer + 1, i);
		});
		menuSave->setFontSize(18);
		auto menuDelete = MenuItemFont::create("Delete", [=](Ref* ref) {
			SaveControl::instance()->deleteSave(i);
			auto subLayer = static_cast<LayerColor*>(SaverLayer->getChildByTag(i));
			subLayer->removeChildByTag(i);
			Label* label = Label::createWithTTF(ttfconfig, "No Save Existed");
			label->setPosition(subLayer->getContentSize().width / 2, subLayer->getContentSize().height / 2);
			subLayer->addChild(label, kZSaveLayer + 1, i);
		});
		menuDelete->setFontSize(18);
		auto menu = Menu::create(menuSave,menuDelete,NULL);
		menu->alignItemsHorizontallyWithPadding(30);
		SaverLayer->addChild(menu);
		menu->setPosition(7.8 * SaverLayer->getContentSize().width / 9, (2 * i)*SaverLayer->getContentSize().height / 10 + 45);

	}

	auto MouseListener = EventListenerMouse::create(); //鼠标事件监听器
	MouseListener->onMouseMove = CC_CALLBACK_1(GameScene::SaveLayerOnMouseMove, this);
	MouseListener->onMouseDown = CC_CALLBACK_1(GameScene::SaveLayerOnMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseListener, SaverLayer);

	auto TouchListener = EventListenerTouchOneByOne::create();
	TouchListener->onTouchBegan = [&](Touch* touch, Event* event) {return true; };
	TouchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListener, SaverLayer);

	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = [&](EventKeyboard::KeyCode keycode, Event* event) {
		if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE) {
			this->setIfShowingSaveLayer(false);
			this->removeChildByTag(kZSaveLayer);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);

}

//获得该处存档的游戏基本信息的std::string
std::string GameScene::createTheInfoShowedOnSaveLayer(int order)
{
	//获得全部文件信息的ValueMap
	auto valueMap = FileUtils::getInstance()->getValueMapFromFile(Global::instance()->fullpath);
	//获得目标存档的Dict值
	auto Saver = valueMap.at("Save" + std::to_string(order)).asValueMap();
	//通过ValueMap的key分别获得英雄和地图的字典
	auto SaverOfHero = Saver.at("SaverOfHero").asValueMap();

	int currentLevel = SaverOfHero.at("currentLevel").asInt();
	int HP = SaverOfHero.at("HP").asInt();
	int ATK = SaverOfHero.at("ATK").asInt();
	int DEF = SaverOfHero.at("DEF").asInt();

	return "Level : " + std::to_string(currentLevel) + " HP : " + std::to_string(HP) + " ATK : " + std::to_string(ATK) + " DEF : " + std::to_string(DEF);
}

//在鼠标移动时 在SaveLayer的存档条处变色
void GameScene::SaveLayerOnMouseMove(Event * event)
{
	auto MouseEvent = (EventMouse*)event;
	float x = MouseEvent->getCursorX();
	float y = MouseEvent->getCursorY();
	for (int i = 0; i < 5; i++) {
		auto subLayer = static_cast<LayerColor*>(this->getChildByTag(kZSaveLayer)->getChildByTag(i));

	

		if (subLayer->getBoundingBox().containsPoint(Vec2(x, y))) {
			Size size = subLayer->getContentSize();
			subLayer->setColor(Color3B::YELLOW);
			auto box = subLayer->getBoundingBox();
			auto minx = box.getMinX();
			auto miny = box.getMinY();
			auto maxx = box.getMaxX();
			auto maxy = box.getMaxY();
			auto y = box.getMinY();
		}
		else {
			subLayer->setColor(Color3B::BLUE);
		}
	}
}

//在鼠标点击后 读档
void GameScene::SaveLayerOnMouseDown(Event * event)
{
	auto MouseEvent = (EventMouse*)event;
	
	float x = MouseEvent->getCursorX();
	float y = MouseEvent->getCursorY();
	for (int i = 0; i < 5; i++) {
		auto subLayer = static_cast<LayerColor*>(this->getChildByTag(kZSaveLayer)->getChildByTag(i));
		if (subLayer->getBoundingBox().containsPoint(Vec2(x, y))) {
			if (SaveControl::instance()->checkIfTheSaveExisted(i)) {
				SaveControl::instance()->load(i);
				this->isShowingSaveLayer = false;
				this->removeChildByTag(kZSaveLayer);
				return;
			}
		}
	}
}

void GameScene::GameVictory()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/VictoryBGM.mp3");
	
	std::string vicMessage = 
		"With the dead of the Dark Lord, lightning shines and loud noises are made. All parts of the tower start to shake. There is a crack appear on the wall. You got into it and finally get out the tower. The Dark Lords who has sliently lived in the tower so long was killed by you, same as the lifes in the tower. You don't know how did you get into the tower, but in order to get out of the tower, you do things at any cost, without even a word asked.\n\nWhich one is the dark one?";

	//创建通关后弹出的图层，并将滚动文本添加至图层中
	victoryLayer = LayerColor::create(Color4B::BLACK);
	TTFConfig vicConfig("fonts/Marker Felt.ttf", 36);
	auto victoryLabel = Label::createWithTTF(vicConfig, vicMessage);
	victoryLabel->setColor(Color3B::WHITE);
	victoryLabel->setAnchorPoint(Vec2::ZERO);
	victoryLabel->setDimensions(400, 700);
	victoryLabel->setPosition(104, -700);
	victoryLayer->addChild(victoryLabel);

	//创建“Continue”键并使其和滚动文本一起滚动，到最后显示出来
	auto continueLabel = Label::createWithTTF(vicConfig, "Continue");
	continueLabel->setColor(Color3B::RED);
	auto* continueButton = Menu::create(MenuItemLabel::create(continueLabel, CC_CALLBACK_0(GameScene::continueCALLBACK, this)), NULL);
	continueButton->setPosition(200, -50);
	victoryLabel->addChild(continueButton);
	
	this->addChild(victoryLayer,500);
	victoryLabel->runAction(MoveTo::create(30.0f, Vec2(104, 100)));


}

void GameScene::GameDefeat()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/DeathBGM.mp3");

	std::string defeatMessage =
		"You become weak and paralyzed, more and more unconsciousness. A strange feel fill in all parts of you, you get into a stupor. When you are awake, you find yourself lying on the bottom floor, with a weak body like the initial situation. All is same as what was said, without defeating the Dark Lord, no one could get out of the tower.";

	//创建死亡后弹出图层并添加滚动文本
	defeatLayer = LayerColor::create(Color4B::BLACK);
	TTFConfig defeatConfig("fonts/Marker Felt.ttf", 36);
	auto defeatLabel = Label::createWithTTF(defeatConfig, defeatMessage);
	defeatLabel->setColor(Color3B::WHITE);
	defeatLabel->setAnchorPoint(Vec2::ZERO);
	defeatLabel->setDimensions(400, 500);
	defeatLabel->setPosition(104, -500);
	defeatLayer->addChild(defeatLabel);

	//添加continue按钮并随文本一同滚动
	auto continueLabel = Label::createWithTTF(defeatConfig, "Continue");
	continueLabel->setColor(Color3B::RED);
	auto* continueButton = Menu::create(MenuItemLabel::create(continueLabel, CC_CALLBACK_0(GameScene::continueCALLBACK, this)), NULL);
	continueButton->setPosition(200, -50);
	defeatLabel->addChild(continueButton);

	this->addChild(defeatLayer, 500);
	defeatLabel->runAction(MoveTo::create(20.0f, Vec2(104, 100)));

}

void GameScene::continueCALLBACK()
{
	Global::instance()->gameMap->removeFromParent();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ButtonBGS.mp3");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	Global::instance()->resetGlobal();
	auto scene = StartScene::createStartScene();
	Director::getInstance()->replaceScene(scene);
}

void GameScene::SaverLayerExitButtonCallBack(Ref * psender)
{
	this->setIfShowingSaveLayer(false);
	this->removeChildByTag(kZSaveLayer);

}


