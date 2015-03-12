
/*
Template
*/


#include "GameClass.h"
//
int main(int argc, char *argv[])
{
	GameClass app;
	while (!app.UpdateAndRender()) {}
	return 0;
}