//******************
//
// scls_demo_raycast.cpp
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The "Demo" part is a part full of demonstrations for SCLS.
//
// This file contains the source code of scls_demo_raycast.h.
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

// Include SCLS library
#include "scls_demo_raycast.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    namespace demo {
        // Fill the map with the needed cases
        void Raycast_Map::fill_map(int width, int height) {
            a_cases.clear();

            // Create each cases
            for(int i = 0;i<static_cast<int>(height);i++) {
                std::vector<Case> current_column;
                for(int j = 0;j<static_cast<int>(width);j++) {
                    Case new_case;
                    current_column.push_back(new_case);
                } a_cases.push_back(current_column);
            }
        }

        // Loads the map from a text
        void Raycast_Map::load_from_text(std::string text) {
            a_cases.clear();

            // Cut the text as needed
            std::vector<std::string> lines = scls::cut_string(text, "-");
            for(int i = 0;i<static_cast<int>(lines.size());i++) {
                std::vector<std::string> column = scls::cut_string(lines[i], ";");
                std::vector<Case> current_column;
                for(int j = 0;j<static_cast<int>(column.size());j++) {
                    Case new_case; new_case.set_number(std::stoi(column[j]));
                    current_column.push_back(new_case);
                } a_cases.push_back(current_column);
            }
        }

        //*********
        //
        // Raycast handling
        //
        //*********

        // Does a raycast in the map
        Raycast_Map::Raycast Raycast_Map::raycast(double start_x, double start_y, double angle_in_degrees, bool ignore_collision) {
            // Calculate the needed datas
            double angle_in_radians = (angle_in_degrees / 180.0) * SCLS_PI;
            bool continue_horizontal = true; bool continue_vertical = true;
            double cos_angle = std::cos(angle_in_radians); double sin_angle = std::sin(angle_in_radians);
            if(cos_angle == 0) cos_angle = 0.000001; if(sin_angle == 0) sin_angle = 0.000001;
            double horizontal_advance = 1;
            if(cos_angle < 0) horizontal_advance = -1;
            Case* horizontal_case = 0;
            double horizontal_ratio = std::abs(sin_angle / cos_angle);
            if(sin_angle < 0) horizontal_ratio = -horizontal_ratio;
            double horizontal_x = 0; double horizontal_y = 0;
            double vertical_advance = 1;
            if(sin_angle < 0) vertical_advance = -1;
            Case* vertical_case = 0;
            double vertical_ratio = std::abs(cos_angle / sin_angle);
            if(cos_angle < 0) vertical_ratio = -vertical_ratio;
            double vertical_x = 0; double vertical_y = 0;

            // Do the Raycast horizontally (through vertical lines, the rays are horizontal)
            double current_y = start_y;
            double to_add = std::ceil(start_x) - start_x;
            if(horizontal_advance < 0) to_add = start_x - std::floor(start_x);
            double current_x = std::ceil(start_x);
            if(horizontal_advance < 0) current_x = std::floor(start_x);
            current_y += horizontal_ratio * to_add;
            // First check
            int used_x = current_x;
            if(horizontal_advance < 0) used_x = current_x - 1;
            int used_y = std::floor(current_y);
            // Do the first test
            if((used_x < 0 || used_x >= width()) || (used_y < 0 || used_y >= height())) {continue_horizontal = false;}
            else if(case_at(used_x, used_y)->number() != 0 && !ignore_collision) {continue_horizontal = false;horizontal_case = case_at(used_x, used_y);}
            else {
                while(continue_horizontal) {
                    // Advance the ray
                    current_x += horizontal_advance;
                    current_y += horizontal_ratio;

                    // See if the ray must be stopped
                    used_x = current_x;
                    if(horizontal_advance < 0) used_x = current_x - 1;
                    used_y = std::floor(current_y);
                    if((used_x < 0 || used_x >= width()) || (used_y < 0 || used_y >= height())) {
                        continue_horizontal = false;
                    } else if(case_at(used_x, used_y)->number() != 0 && !ignore_collision) {
                        continue_horizontal = false;
                        horizontal_case = case_at(used_x, used_y);
                    }
                }
            } horizontal_x = current_x; horizontal_y = current_y;

            // Do the Raycast vertical (through horizontal lines, the rays are vertical)
            current_x = start_x;
            to_add = std::ceil(start_y) - start_y;
            if(vertical_advance < 0) to_add = start_y - std::floor(start_y);
            current_x += vertical_ratio * to_add;
            current_y = std::ceil(start_y);
            if(vertical_advance < 0) current_y = std::floor(start_y);
            // First check
            used_x = std::floor(current_x);
            used_y = current_y;
            if(vertical_advance < 0) used_y = current_y - 1;
            // Do the first test
            if((used_x < 0 || used_x >= width()) || (used_y < 0 || used_y >= height())) {continue_vertical = false;}
            else if(case_at(used_x, used_y)->number() != 0 && !ignore_collision) {continue_vertical = false;vertical_case = case_at(used_x, used_y);}
            else {
                while(continue_vertical) {
                    // Advance the ray
                    current_x += vertical_ratio;
                    current_y += vertical_advance;

                    // See if the ray must be stopped
                    used_x = std::floor(current_x);
                    used_y = current_y;
                    if(vertical_advance < 0) used_y = current_y - 1;
                    if((used_x < 0 || used_x >= width()) || (used_y < 0 || used_y >= height())) {
                        continue_vertical = false;
                    }
                    else if(case_at(used_x, used_y)->number() != 0 && !ignore_collision) {
                        continue_vertical = false;
                        vertical_case = case_at(used_x, used_y);
                    }
                }
            } vertical_x = current_x; vertical_y = current_y;

            // Prepare the result
            double distance_horizontal = std::sqrt(std::pow(start_x - horizontal_x, 2) + std::pow(start_y - horizontal_y, 2));
            double distance_vertical = std::sqrt(std::pow(start_x - vertical_x, 2) + std::pow(start_y - vertical_y, 2));
            Raycast_Map::Raycast to_return;
            // Chose the good collision
            if(distance_horizontal < distance_vertical) {
                to_return.set_collision_case(horizontal_case);
                to_return.set_collision_distance(distance_horizontal);
                to_return.set_collision_x(horizontal_x);
                to_return.set_collision_y(horizontal_y);
            } else {
                to_return.set_collision_case(vertical_case);
                to_return.set_collision_distance(distance_vertical);
                to_return.set_collision_x(vertical_x);
                to_return.set_collision_y(vertical_y);
            }
            return to_return;
        }

        //*********
        //
        // Raycast_Engine mains members
        //
        //*********

        // Render the 2D part of the engine
        std::shared_ptr<__Image_Base> Raycast_Engine::render_2d() {
            std::shared_ptr<__Image_Base> to_return = std::make_shared<__Image_Base>(500, 500, scls::Color(255, 255, 255));

            std::map<int, scls::Color> colors;
            colors[0] = scls::Color(255, 255, 255);
            colors[1] = scls::Color(255, 0, 0);
            colors[2] = scls::Color(0, 255, 0);
            colors[3] = scls::Color(0, 0, 255);

            // Draw the current map
            int case_height = to_return.get()->height() / current_map()->height();
            int case_width = to_return.get()->width() / current_map()->width();
            for(int i = 0;i<static_cast<int>(current_map()->height());i++) {
                for(int j = 0;j<static_cast<int>(current_map()->width());j++) {
                    to_return.get()->fill_rect(j * case_width, i * case_height, case_width, case_height, colors[current_map()->case_at(j, i)->number()]);
                }
            }

            // Draw the delimitations
            int delimitation_width = 2;
            for(int i = 0;i<static_cast<int>(current_map()->width()) + 1;i++) {
                to_return.get()->fill_rect(i * case_width - delimitation_width / 2.0, 0, delimitation_width, to_return.get()->height(), scls::Color(0, 0, 0));
            }
            for(int i = 0;i<static_cast<int>(current_map()->height()) + 1;i++) {
                to_return.get()->fill_rect(0, i * case_height - delimitation_width / 2.0, to_return.get()->width(), delimitation_width, scls::Color(0, 0, 0));
            }

            // Calculate the datas for the camera
            double camera_width = case_width / 2.0;
            double camera_x = a_camera.x();
            double camera_y = a_camera.z();

            // Draw the camera axis
            //to_return.get()->fill_rect(camera_x * case_width, 0, delimitation_width, to_return.get()->height(), scls::Color(0, 0, 255));
            //to_return.get()->fill_rect(0, camera_y * case_width - delimitation_width / 2.0, to_return.get()->width(), delimitation_width, scls::Color(0, 0, 255));

            // Draw the rays
            double base_angle = -a_camera.rotation_y() + 90;
            double fov = 45;
            double rays_number = 100;
            for(int i = 0;i<static_cast<int>(rays_number);i++) {
                // Draw a single ray
                Raycast_Map::Raycast result = current_map()->raycast(a_camera.x(), a_camera.z(), base_angle + (fov / 2.0 - (fov / rays_number) * i));
                to_return.get()->draw_line(camera_x * case_width, camera_y * case_height, result.collision_x() * case_width, result.collision_y() * case_height, scls::Color(0, 0, 255));
            } //*/

            // Draw the camera
            to_return.get()->fill_rect(camera_x * case_width - (camera_width / 2.0), camera_y * case_height - (camera_width / 2.0), camera_width, camera_width, scls::Color(0, 128, 255));

            // Take a screenshot
            if(window_struct()->key_pressed_during_this_frame("p")) {to_return.get()->save_png("tests/rays_2d.png");}

            return to_return;
        }

        // Render the 3D part of the engine
        scls::Image img;bool img_used = false;
        std::shared_ptr<__Image_Base> Raycast_Engine::render_3d() {
            // Img
            if(!img_used){img = scls::Image("assets/cobble.png");img_used=true;}

            std::shared_ptr<__Image_Base> to_return = std::make_shared<__Image_Base>(window_struct()->window_width(), window_struct()->window_height(), scls::Color(255, 255, 255));

            // Draw the ground and the sky
            to_return.get()->fill_rect(0, 0, to_return.get()->width(), to_return.get()->height() / 2.0, scls::Color(125, 125, 255));

            double base_angle = -a_camera.rotation_y() + 90;
            double fov = 45;
            double rays_number = to_return.get()->width();
            double ray_width_in_pixel = to_return.get()->width() / rays_number;
            for(int i = 0;i<static_cast<int>(rays_number);i++) {
                // Get the needed ray
                Raycast_Map::Raycast result = current_map()->raycast(a_camera.x(), a_camera.z(), base_angle + (-fov / 2.0 + (fov / rays_number) * i));

                // Draw the result
                double block_height = to_return.get()->height() * (1.0 / result.collision_distance());
                double block_y = to_return.get()->height() / 2.0 - block_height/ 2.0;

                //if(result.collision_case() != 0){to_return.get()->fill_rect(i * ray_width_in_pixel, block_y, ray_width_in_pixel, block_height, scls::Color(255, 0, 0));}
                if(result.collision_case() != 0){to_return.get()->paste(img.resize_adaptative(ray_width_in_pixel, block_height), i * ray_width_in_pixel, block_y);}
                else{to_return.get()->fill_rect(i * ray_width_in_pixel, block_y, ray_width_in_pixel, block_height, scls::Color(0, 255, 0));}
            }

            // Take a screenshot
            if(window_struct()->key_pressed_during_this_frame("p")) {to_return.get()->save_png("tests/3d.png");}

            return to_return;
        }

        // Update the camera in the engine
        void Raycast_Engine::update_camera() {
            if(window_struct()->key_state("s") == scls::Key_State::Pressed) {a_camera.move_forward(-window_struct()->delta_time());}
            if(window_struct()->key_state("z") == scls::Key_State::Pressed) {a_camera.move_forward(window_struct()->delta_time());}
            if(window_struct()->key_state("q") == scls::Key_State::Pressed) {a_camera.move_right(-window_struct()->delta_time());}
            if(window_struct()->key_state("d") == scls::Key_State::Pressed) {a_camera.move_right(window_struct()->delta_time());}

            if(window_struct()->key_state("a") == scls::Key_State::Pressed) {a_camera.set_rotation_y(a_camera.rotation_y() + window_struct()->delta_time() * 70);}
            if(window_struct()->key_state("e") == scls::Key_State::Pressed) {a_camera.set_rotation_y(a_camera.rotation_y() - window_struct()->delta_time() * 70);}
        }

        // Use the Raycast window
        void use_raycast(int window_width, int window_height, std::string exec_path) {
            std::unique_ptr<__Temp_Window> raycast = std::make_unique<__Temp_Window>(window_width, window_height, exec_path);

            // Create the needed page
            std::shared_ptr<GUI_Page> page = raycast.get()->new_page_2d<GUI_Page>("raycast");
            raycast.get()->display_page_2d("raycast");

            // Create the engine
            Raycast_Engine engine(raycast.get());
            std::shared_ptr<Raycast_Map> map_1 = engine.new_map();
            // Create the map
            std::string needed_text = "";
            needed_text += "0;0;0;0;1;0;0;0;0;0-";
            needed_text += "0;0;0;0;1;0;0;0;0;0-";
            needed_text += "0;0;0;0;1;0;0;0;0;0-";
            needed_text += "0;1;1;1;1;0;1;1;1;0-";
            needed_text += "0;0;0;0;0;0;1;0;0;0-";
            needed_text += "0;0;0;0;0;0;1;0;0;0-";
            needed_text += "0;1;1;1;1;0;1;1;1;0-";
            needed_text += "0;0;0;0;1;0;0;0;0;0-";
            needed_text += "0;0;0;0;1;0;0;0;0;0-";
            needed_text += "0;0;0;0;1;0;0;0;0;0";
            map_1.get()->load_from_text(needed_text);
            engine.set_current_map_shared_ptr(map_1);

            // Execution loop
            while(raycast.get()->run()) {
                raycast.get()->update_event();

                // Render the raycast
                engine.update_camera();
                std::shared_ptr<__Image_Base> img = engine.render_3d();
                page.get()->parent_object()->texture()->set_image(img);
                page.get()->parent_object()->set_should_render_during_this_frame(true);
                img.reset();

                raycast.get()->update();
                raycast.get()->render();
            }
        }
    }
}
