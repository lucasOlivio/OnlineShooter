#include <Engine/Engine.h>

int main(int argc, char** argv)
{
	GetEngine().Initialize(argc, argv);

	GetEngine().Run();

	GetEngine().Destroy();

	return 0;
}