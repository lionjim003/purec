@..\mingw64_gcc12.2.0\bin\gcc.exe -std=c17 -static -Wall -o a.exe code.c ^
-I../vulkansdk_1.3.239.0/Include -I../glfw_3.3.8/include -I../cglm_0.9.0/include ^
-L../vulkansdk_1.3.239.0/Lib -lvulkan-1 -L../glfw_3.3.8/lib-mingw-w64 -lglfw3 -lgdi32 ^ 
