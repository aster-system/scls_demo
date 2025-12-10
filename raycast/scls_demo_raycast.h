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
// This file contains a little raycast engine made with SCLS.
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
        //*********
        //
        // The Raycast_Map class
        //
        //*********

        class Raycast_Map {
            // Class representating a raycast map
        public:

            //*********
            //
            // Case in the map
            //
            //*********

            class Case {
                // Class representating a case in the map
            public:

                // Create a case
                Case(){};

                // Getters and setters
                inline int number() const {return a_number;};
                inline void set_number(int new_number) {a_number = new_number;};

            private:
                // Number of the case
                int a_number = 0;
            };

            //*********
            //
            // Raycast_Map mains members
            //
            //*********

            // Create a Raycast_Map
            Raycast_Map(){};
            // Returns the case at a certain position
            inline Case* case_at(int x, int y) {if(x<0||y<0)return 0;return &a_cases[y][x]; };
            // Fills the map with the needed cases
            void fill_map(int width, int height);
            // Loads the map from a text
            void load_from_text(std::string text);

            // Getters and setters
            inline const std::vector<std::vector<Case>>& cases() const {return a_cases;};
            inline int height() const {if(a_cases.size()<=0)return 0;return a_cases.at(0).size();};
            inline int width() const {return a_cases.size();};

            //*********
            //
            // Raycast handling
            //
            //*********

            class Raycast {
                // Class representating the result of a raycast
            public:
                // Raycast constructor
                Raycast(){};

                // Getters and setters
                inline Case* collision_case() const {return a_collision_case;};
                inline double collision_distance() const {return a_collision_distance;};
                inline double collision_x() const {return a_collision_x;};
                inline double collision_y() const {return a_collision_y;};
                inline void set_collision_case(Case* new_collision_case){a_collision_case=new_collision_case;};
                inline void set_collision_distance(double new_collision_distance){a_collision_distance = new_collision_distance;};
                inline void set_collision_x(double new_collision_x){a_collision_x = new_collision_x;};
                inline void set_collision_y(double new_collision_y){a_collision_y = new_collision_y;};

            private:
                // Case touched by the collision
                Case* a_collision_case = 0;
                // Distance of the collision
                double a_collision_distance = 0;
                // X position of the collision
                double a_collision_x = 0;
                // Y position of the collision
                double a_collision_y = 0;
            };

            // Does a raycast in the map
            Raycast raycast(double start_x, double start_y, double angle_in_degrees, bool ignore_collision);
            inline Raycast raycast(double start_x, double start_y, double angle_in_degrees) {return raycast(start_x, start_y, angle_in_degrees, false);};

        public:
            // Each cases of the Map
            std::vector<std::vector<Case>> a_cases;
        };

        //*********
        //
        // The Raycast_Engine class
        //
        //*********

        class Raycast_Engine {
            // Class representating a raycast engine
        public:

            //*********
            //
            // Raycast_Engine mains members
            //
            //*********

            // Create a Raycast_Engine
            Raycast_Engine(scls::Window* window_struct):a_window_struct(window_struct){a_camera.set_x(0.5);a_camera.set_z(0.5);a_camera.set_rotation_y(0);};

            // Create a new map in the engine
            inline std::shared_ptr<Raycast_Map> new_map(){std::shared_ptr<Raycast_Map> current_map = std::make_shared<Raycast_Map>();a_maps.push_back(current_map);return current_map;};

            // Render the 2D part of the engine
            std::shared_ptr<__Image_Base> render_2d();
            // Render the 3D part of the engine
            std::shared_ptr<__Image_Base> render_3d();
            // Update the camera in the engine
            void update_camera();

            // Getters and setters
            inline Raycast_Map* current_map() const {return a_current_map.get();};
            inline void set_current_map_shared_ptr(std::shared_ptr<Raycast_Map> new_current_map) {a_current_map = new_current_map;};
            inline scls::Window* window_struct() const {return a_window_struct;};

        private:
            // Position of the camera in the map
            Transform_Object_3D a_camera;
            // Current map used in the engine
            std::shared_ptr<Raycast_Map> a_current_map;
            // Maps of the engine
            std::vector<std::shared_ptr<Raycast_Map>> a_maps;
            // Struct of the window
            scls::Window* const a_window_struct;
        };

        class __Temp_Window : public scls::Window {
            // Temporary used window by SCLS Demo
        public :
            // __Temp_Window constructor
            __Temp_Window(unsigned int w, unsigned int h, std::string path) : scls::Window(w, h, path) { set_window_title("Raycast"); }

            // Create an object in the window
            std::shared_ptr<scls::Object> __create_loaded_object_from_type(std::string object_name, std::string object_type) {
                return scls::Window::__create_loaded_object_from_type(object_name, object_type);
            }
        };

        // Use the Raycast window
        void use_raycast(int window_width, int window_height, std::string exec_path);
    }
}

#endif //SCLS_DEMO_RAYCAST
