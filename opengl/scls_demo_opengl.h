//******************
//
// scls_demo_raycast.h
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The "Demo" part is a part full of demonstrations for SCLS.
//
// This file contains a little OpenGL engine made with SCLS.
//
//******************
//
// License (GPL V3.0) :
//
// Copyright (C) 2024 by Aster System, Inc. <https://aster-system.github.io/aster-system/>
// This file is part of SCLS.
// SCLS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// SCLS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with SCLS. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef SCLS_DEMO_RAYCAST
#define SCLS_DEMO_RAYCAST

// Include SCLS library
#include "../../scls-graphic-benoit/scls_graphic.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    namespace demo {

        class OpenGL_Object : public scls::Object {
            // Class representating an OpenGL object
        public:

            // OpenGL_Object most basic constructor
            OpenGL_Object(_Window_Advanced_Struct* window_struct, std::string name):Object(window_struct, name){};
            // OpenGL_Object destructor
            virtual ~OpenGL_Object() {};
        };

        //*********
        //
        // The OpenGL_Engine class
        //
        //*********

        class OpenGL_Engine {
            // Class representating an OpenGL engine
        public:

            //*********
            //
            // OpenGL_Engine mains members
            //
            //*********

            // Create an OpenGL_Engine
            OpenGL_Engine(scls::Window* window_struct):a_window_struct(window_struct){};

        private:
            // Structure of the window
            scls::Window* const a_window_struct;
        };

        class __Temp_Window : public scls::Window {
            // Temporary used window by SCLS Demo
        public :
            // __Temp_Window constructor
            __Temp_Window(unsigned int w, unsigned int h, std::string path) : scls::Window(w, h, path) { set_window_title("Raycast"); }

            // Create an object in the window
            std::shared_ptr<scls::Object> __create_loaded_object_from_type(std::string object_name, std::string object_type) {return scls::Window::__create_loaded_object_from_type(object_name, object_type);};
        };

        // Use the OpenGL window
        void use_opengl(int window_width, int window_height, std::string exec_path);
    }
}

#endif //SCLS_DEMO_RAYCAST
