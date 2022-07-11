#include "vecop.h"
/* IMVEC2 OPERATIONS */
ImVec2 operator+(const ImVec2 & l,const ImVec2 & r) {   
    return {l.x+r.x,l.y+r.y};                                    
}
                                                
ImVec2 operator-(const ImVec2& l,const ImVec2 & r) {   
    return {l.x-r.x,l.y-r.y};                                    
}

ImVec2 operator+(const ImVec2 & l, const float& f) {
    return {l.x+f, l.y+f};
}

ImVec2 operator-(const ImVec2 & l, const float& f) {
    return {l.x-f, l.y-f};
}

ImVec2 operator*(const ImVec2 & l, const float& f) {
    return {l.x*f, l.y*f};
}

ImVec2 operator/(const ImVec2 & l, const float& f) {
    return {l.x/f, l.y/f};
}

