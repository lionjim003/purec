@gcc -std=c17 -static -Wall %* -o a.exe code.c ^
-I../vulkansdk/Include -I../glfw/include -I../cglm/include ^
-L../vulkansdk/Lib -lvulkan-1 -L../glfw/lib-mingw-w64 -lglfw3 -lgdi32 ^ 
