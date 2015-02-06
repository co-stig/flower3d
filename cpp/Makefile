all : compile link

link :
	g++ -o flower3 flowerpot.o center_gen1.o matrix3x3.o petal.o petal_gen1.o petal_gen2.o petal_turn.o smooth_double.o texturizer.o trunk.o vector3d.o lesson6.o image_DXT.o image_helper.o SOIL.o stb_image_aug.o -lopengl32 -lglu32 -lwinmm -lgdi32
 
compile : 
	g++ -c center_gen1.cpp flowerpot.cpp matrix3x3.cpp petal.cpp petal_gen1.cpp petal_gen2.cpp petal_turn.cpp smooth_double.cpp texturizer.cpp trunk.cpp vector3d.cpp lesson6.cpp image_DXT.c image_helper.c SOIL.c stb_image_aug.c
 