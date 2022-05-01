This project is designed to be able to perform a full n-axis simulation of robot control

It has an inbuilt rendering pipeline and model loader

Can compile with `mingw32-make` - All required files are stored in the repo, but you also need to have OpenGl installed 

The .vscode pipeline has been set up to build the makefile as well as the debugger which can be run with `ctr`+`shift`+`b` and `F5` respectively

The rendering pipeline has been created using the tutorials from [LearningOpenGL](https://learnopengl.com/)

As well the interface utilises the [ImGui::Dockspace Framework](https://github.com/ocornut/imgui/tree/docking)