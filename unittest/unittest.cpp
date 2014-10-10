#include "gtest/include/gtest/gtest.h"
#include "bluemars.h"
#include "flat.h"

TEST(BlueMarsTest, KnownCoord)
{
	BlueMarsMap map;
	map.addLayer("flat", std::make_shared<Flat> (Flat()));
	float value = map.get("flat", 0, 0);
	ASSERT_EQ(value,0); // 0+0
	value = map.get("flat", 10, 10);
	ASSERT_EQ(value,20); //10+10
	value = map.get("flat", 2000, 2000);
	ASSERT_EQ(value,1952);
	value = map.get("flat",1000,2000);
	ASSERT_EQ(value,1976);
}

TEST(BlueMarsTest, NBChunks)
{
	BlueMarsMap map;
	map.addLayer("flat", std::make_shared<Flat> (Flat()));
	map.addLayer("flat2", std::make_shared<Flat> (Flat()));
	ASSERT_EQ(map.nbChunks(),0);
	map.get("flat",0,0);
	map.get("flat2",0,0);
	ASSERT_EQ(map.nbChunks(),2);
	map.get("flat",1000,1000);
	ASSERT_EQ(map.nbChunks(),2);
	map.get("flat",2000,2000);
	ASSERT_EQ(map.nbChunks(),3);

}
	
