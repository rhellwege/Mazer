#pragma once
#include "imgui.h"
#include <utility>
/* IMVEC2 OPERATIONS */
ImVec2 operator+(const ImVec2 & l,const ImVec2 & r);
ImVec2 operator-(const ImVec2& l,const ImVec2 & r);
ImVec2 operator+(const ImVec2 & l, const float& f);
ImVec2 operator-(const ImVec2 & l, const float& f);
ImVec2 operator*(const ImVec2 & l, const float& f);
ImVec2 operator/(const ImVec2 & l, const float& f);