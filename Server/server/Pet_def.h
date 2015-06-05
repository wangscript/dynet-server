#ifndef PET_DEF_H_
#define PET_DEF_H_


//宠物的相关定义

//性格
enum PetPersonality
{

	// 攻击型
	PetAlityAttackHard			= 1,// 实干=努力
	PetAlityAttackAlone			= 2,// 孤僻=寂寞
	PetAlityAttackHold			= 3,// 固执=性格强
	PetAlityAttackNaughty		= 4,// 调皮=任性
	PetAlityAttackBrave			= 5,// 勇敢=勇敢

	// 防御型
	PetAlityDefanseFrank		= 11 ,//坦率=和平
	PetAlityDefanseBold			= 12 ,//大胆=大胆
	PetAlityDefanseNaughty		= 13 ,//淘气=顽皮
	PetAlityDefanseNoWorry		= 14 ,//无虑=轻浮
	PetAlityDefanseLeisurely	= 15 ,//悠闲=慢性

	// 特攻型
	PetAlityTAttackShy			= 21,//害羞=怕人
	PetAlityTAttackKeep			= 22,//保守=客气
	PetAlityTAttackSteady		= 23,//稳重=大方
	PetAlityTAttackCareless		= 24,//马虎=轻率
	PetAlityTAttackChill		= 25,//冷静=冷静

	// 特防型
	PetAlityTdefanseImpetuous	= 31,//浮躁=混乱
	PetAlityTdefanseSteady		= 32,//沉着=沉稳
	PetAlityTdefanseMeek		= 33,//温顺=听话
	PetAlityTdefanseCarefully	= 34,//慎重=乖巧
	PetAlityTdefanseArrogance	= 35,//狂妄=自大

	// 敏捷型
	PetAlitySpeedSerious		= 41,//认真=坚定 
	PetAlitySpeedTimid			= 42,//胆小=胆小
	PetAlitySpeedIrritable		= 43,//急躁=小气
	PetAlitySpeedOptimistic		= 44,//开朗=活泼
	PetAlitySpeedNaive			= 45,//天真=单纯
};




#endif