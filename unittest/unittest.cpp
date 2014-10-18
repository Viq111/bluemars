#include "gtest/include/gtest/gtest.h"
#include "bluemars.h"
#include "simpleAdditionLayer.h"
#include "perlin.h"

TEST(BlueMarsTest, KnownCoord)
{
	BlueMarsMap map;
	map.addLayer("simpleAdditionLayer", std::make_shared<simpleAdditionLayer> (simpleAdditionLayer()));
	float value = map.get("simpleAdditionLayer", 0, 0);
	ASSERT_EQ(value,0); // 0+0
	value = map.get("simpleAdditionLayer", 10, 10);
	ASSERT_EQ(value,20); //10+10
	value = map.get("simpleAdditionLayer", 2000, 2000);
	ASSERT_EQ(value,1952);
	value = map.get("simpleAdditionLayer",1000,2000);
	ASSERT_EQ(value,1976);
	value = map.get("simpleAdditionLayer", -10, 5);
	ASSERT_EQ(value, 1019);
	value = map.get("simpleAdditionLayer", -10, -10);
	ASSERT_EQ(value, 2028);
	value = map.get("simpleAdditionLayer", -2000, -2000);
	ASSERT_EQ(value,96);
	value = map.get("simpleAdditionLayer", 2000, -2000);
	ASSERT_EQ(value,1024);
}

TEST(BlueMarsTest, NBChunks)
{
	BlueMarsMap map;
	map.addLayer("simpleAdditionLayer", std::make_shared<simpleAdditionLayer> (simpleAdditionLayer()));
	map.addLayer("simpleAdditionLayer2", std::make_shared<simpleAdditionLayer> (simpleAdditionLayer()));
	ASSERT_EQ(map.nbChunks(),0);
	map.get("simpleAdditionLayer",0,0);
	map.get("simpleAdditionLayer2",0,0);
	ASSERT_EQ(map.nbChunks(),2);
	map.get("simpleAdditionLayer",1000,1000);
	ASSERT_EQ(map.nbChunks(),2);
	map.get("simpleAdditionLayer",2000,2000);
	ASSERT_EQ(map.nbChunks(),3);
	map.get("simpleAdditionLayer", -10, -10);
	ASSERT_EQ(map.nbChunks(), 4);
	map.get("simpleAdditionLayer", -2000, -2000);
	ASSERT_EQ(map.nbChunks(), 5);
	map.get("simpleAdditionLayer", -2000, 2000);
	ASSERT_EQ(map.nbChunks(), 6);
	map.get("simpleAdditionLayer", -1000, -1000);
}

TEST(PerlinTest, NoiseFunction)
{
    Noise noise1(1);
    Noise noise2(1);
    double X = 384;
    double Y = 117;
    double noise1ForXY = noise1.outputValue(X,Y,5);
    double noise1forYX = noise1.outputValue(Y,X,5);
    double noise2ForXY = noise2.outputValue(X,Y,5);
    ASSERT_EQ(noise1ForXY, noise2ForXY);
    EXPECT_NE(noise1ForXY, noise1ForYX);
}
	
