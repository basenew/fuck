#pragma once

namespace nav{

class SchemeManager
{
public:
    static MapManager& get_instance(){
        static MapManager singleton;
        return singleton;
    }
private:
	SchemeManager(){};
};

} 
