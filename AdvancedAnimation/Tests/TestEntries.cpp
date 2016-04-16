#include "../Framework/Test.h"
#include "../Framework/Render.h"
#include "../Framework/Main.h"
#include "../Framework/ParticleParameter.h"
#include "../Framework/ParticleEmitter.h"

#include "../../freeglut/GL/freeglut.h"

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include "Scene6.h"
#include "Scene7.h"
#include "Scene8.h"
#include "Scene9.h"

TestEntry g_testEntries[] ={
	{ "Scene1", Scene1::Create},
	{ "Scene2", Scene2::Create},
	{ "Scene3", Scene3::Create},
	{ "Scene4", Scene4::Create},
	{ "Scene5", Scene5::Create},
	{ "Scene6", Scene6::Create },
	{ "Scene7", Scene7::Create },
	{ "Scene8", Scene8::Create },
	{ "Scene9", Scene9::Create },
	{NULL, NULL}
};
