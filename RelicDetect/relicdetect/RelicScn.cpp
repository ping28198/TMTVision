#include "RelicScn.h"
bool RelicScn::Match_a_Obj(InputArray obj_descriptors)
{
	FlannBasedMatcher matcher;
	vector<DMatch> matches;

	matcher.match(obj_descriptors, this->descriptors, matches);
	vector<DMatch> good_matches = Get_Good_Matches(matches);
}