#pragma once

namespace nav{

class MapManager
{
public:
    static MapManager& get_instance(){
        static MapManager singleton;
        return singleton;
    }
private:
	MapManager(){};
};

} 
