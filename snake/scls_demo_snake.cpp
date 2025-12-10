//******************
//
// scls_demo_snake.cpp
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The "Demo" part is a part full of demonstrations for SCLS.
//
// This file contains the code source of scls_demo_snake.cpp.
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
#include "scls_demo_snake.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    namespace demo {

        //*********
        //
        // Snake_Total class
        //
        //*********

        // Snake_Total constructor
        Snake_Total::Snake_Total(Window& window, std::string name) : a_name(name), a_window(window) {
            pieces_x().push_back(0);
            pieces_y().push_back(0);
        }

        //*********
        //
        // The Snake_Object class
        //
        //*********

        // Snake_Object constructor
        Snake_Object::Snake_Object(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : GUI_Object(window, name, parent) { }

        //*********
        //
        // The Snake_Piece class
        //
        //*********

        // Snake_Piece constructor
        Snake_Piece::Snake_Piece(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : Snake_Object(window, name, parent) {
            set_background_color(Color(51, 255, 51));
            set_texture_alignment(Alignment_Texture::T_Fit);
            set_type("snake");
        }

        //*********
        //
        // Snake class
        //
        //*********

        // Snake constructor
        Snake::Snake(int window_width, int window_height, std::string exec_path) : Window(window_width, window_height, exec_path) {
            set_background_color(Color(0, 0, 0));

            glfwSetWindowTitle(window(), "Snake par Aster Système");

            // Create the page
            a_gui = *new_page_2d<GUI_Page>("gui");
            a_gui.get()->set_scale(glm::vec3(2, 2, 1));
            a_gui.get()->parent_object()->set_background_color(scls::Color(0, 0, 0));
            a_gui.get()->parent_object()->set_position_in_pixel(0, 0);
            a_gui.get()->parent_object()->set_height_in_scale(scls::Fraction(1));
            a_gui.get()->parent_object()->set_width_in_scale(scls::Fraction(1));
            display_page_2d("gui");

            // Create the playground
            a_playground = *a_gui.get()->parent_object()->new_object<GUI_Object>("playground");
            a_playground->set_height_in_pixel(window_width);
            a_playground->set_width_in_scale(1);
            a_playground->set_y_in_object_scale(Fraction(3, 5));
            std::shared_ptr<Image> image = playground_image();
            a_playground->texture()->set_image(image);
            a_playground->move_left_in_parent();

            // Logo of Aster System
            a_logo = *a_gui.get()->parent_object()->new_object<GUI_Object>("aster_system_logo");
            a_logo->set_height_in_scale(Fraction(1, 8));
            a_logo->set_width_in_scale(1);
            a_logo->set_texture_alignment(Alignment_Texture::T_Fit);
            image = aster_system_logo();
            a_logo->texture()->set_image(image);
            a_logo->move_bottom_in_parent(30);
            a_logo->set_x_in_object_scale(Fraction(1, 2));

            // Lose text
            a_lose_text = *a_gui->parent_object()->new_object<GUI_Text>("lose_text");
            a_lose_text->set_background_color(Color(0, 0, 0, 0));
            a_lose_text->set_font_color(Color(255, 255, 255));
            a_lose_text->set_font_size(50);
            a_lose_text->set_height_in_pixel(50);
            a_lose_text->set_text("Vous avez perdu");
            a_lose_text->set_width_in_scale(1);
            a_lose_text->set_texture_alignment(Alignment_Texture::T_Fit);
            a_lose_text->move_left_in_parent();
            a_lose_text->move_bottom_of_object_in_parent(a_playground, 10);

            // Restart button
            a_restart_button = *a_gui.get()->parent_object()->new_object<GUI_Text>("restart_button");
            a_restart_button->set_background_color(Color(160, 160, 160, 160));
            a_restart_button->set_font_color(Color(0, 0, 0));
            a_restart_button->set_font_size(50);
            a_restart_button->set_height_in_pixel(50);
            a_restart_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
            a_restart_button->set_text("Recommencer");
            a_restart_button->set_width_in_scale(1);
            a_restart_button->set_texture_alignment(Alignment_Texture::T_Fit);
            a_restart_button->move_left_in_parent();
            a_restart_button->move_bottom_of_object_in_parent(a_lose_text, 10);

            // Load the score
            a_score = *a_gui.get()->parent_object()->new_object<GUI_Text>("score");
            a_score->set_background_color(Color(0, 0, 0, 0));
            a_score->set_font_color(Color(255, 255, 255));
            a_score->set_font_size(50);
            a_score->set_height_in_pixel(50);
            a_score->set_width_in_scale(1);
            a_score->set_texture_alignment(Alignment_Texture::T_Fit);
            a_score->move_left_in_parent();
            a_score->move_top_in_parent(10);

            // Create the textures
            // Bottom snake texture
            a_bottom_snake_head_texture = std::make_shared<scls::__Image_Base>(100, 100, Color(0, 0, 0, 0));
            a_bottom_snake_head_texture.get()->fill_rect(10, 63, 25, 25, Color(255, 255, 255, 255));
            a_bottom_snake_head_texture.get()->fill_rect(65, 63, 25, 25, Color(255, 255, 255, 255));
            a_bottom_snake_head_texture.get()->fill_rect(15, 73, 15, 15, Color(0, 0, 0, 255));
            a_bottom_snake_head_texture.get()->fill_rect(70, 73, 15, 15, Color(0, 0, 0, 255));
            // Left snake texture
            a_left_snake_head_texture = std::make_shared<scls::__Image_Base>(100, 100, Color(0, 0, 0, 0));
            a_left_snake_head_texture.get()->fill_rect(12 , 10, 25, 25, Color(255, 255, 255, 255));
            a_left_snake_head_texture.get()->fill_rect(12 , 65, 25, 25, Color(255, 255, 255, 255));
            a_left_snake_head_texture.get()->fill_rect(12 , 15, 15, 15, Color(0, 0, 0, 255));
            a_left_snake_head_texture.get()->fill_rect(12 , 70, 15, 15, Color(0, 0, 0, 255));
            // Right snake texture
            a_right_snake_head_texture = std::make_shared<scls::__Image_Base>(100, 100, Color(0, 0, 0, 0));
            a_right_snake_head_texture.get()->fill_rect(63, 10, 25, 25, Color(255, 255, 255, 255));
            a_right_snake_head_texture.get()->fill_rect(63, 65, 25, 25, Color(255, 255, 255, 255));
            a_right_snake_head_texture.get()->fill_rect(73, 15, 15, 15, Color(0, 0, 0, 255));
            a_right_snake_head_texture.get()->fill_rect(73, 70, 15, 15, Color(0, 0, 0, 255));
            // Top snake texture
            a_top_snake_head_texture = std::make_shared<scls::__Image_Base>(100, 100, Color(0, 0, 0, 0));
            a_top_snake_head_texture.get()->fill_rect(10, 12, 25, 25, Color(255, 255, 255, 255));
            a_top_snake_head_texture.get()->fill_rect(65, 12, 25, 25, Color(255, 255, 255, 255));
            a_top_snake_head_texture.get()->fill_rect(15, 12, 15, 15, Color(0, 0, 0, 255));
            a_top_snake_head_texture.get()->fill_rect(70, 12, 15, 15, Color(0, 0, 0, 255));
            // Dead head texture
            // Texture of the bottom of the head of the snake
            a_bottom_snake_dead_head_texture = std::make_shared<scls::__Image_Base>(100, 100, Color(0, 0, 0, 0));
            a_bottom_snake_dead_head_texture.get()->draw_line(10, 65, 35, 90, Color(0, 0, 0, 255), 7);
            a_bottom_snake_dead_head_texture.get()->draw_line(35, 65, 10, 90, Color(0, 0, 0, 255), 7);
            a_bottom_snake_dead_head_texture.get()->draw_line(90, 65, 65, 90, Color(0, 0, 0, 255), 7);
            a_bottom_snake_dead_head_texture.get()->draw_line(65, 65, 90, 90, Color(0, 0, 0, 255), 7);
            // Texture of the left of the head of the snake
            a_left_snake_dead_head_texture = std::make_shared<Image>(100, 100, Color(0, 0, 0, 0));
            a_left_snake_dead_head_texture.get()->draw_line(10, 10, 35, 35, Color(0, 0, 0, 255), 7);
            a_left_snake_dead_head_texture.get()->draw_line(10, 35, 35, 10, Color(0, 0, 0, 255), 7);
            a_left_snake_dead_head_texture.get()->draw_line(10, 90, 35, 65, Color(0, 0, 0, 255), 7);
            a_left_snake_dead_head_texture.get()->draw_line(10, 65, 35, 90, Color(0, 0, 0, 255), 7);
            // Texture of the left of the head of the snake
            a_right_snake_dead_head_texture = std::make_shared<Image>(100, 100, Color(0, 0, 0, 0));
            a_right_snake_dead_head_texture.get()->draw_line(65, 10, 90, 35, Color(0, 0, 0, 255), 7);
            a_right_snake_dead_head_texture.get()->draw_line(65, 35, 90, 10, Color(0, 0, 0, 255), 7);
            a_right_snake_dead_head_texture.get()->draw_line(65, 90, 90, 65, Color(0, 0, 0, 255), 7);
            a_right_snake_dead_head_texture.get()->draw_line(65, 65, 90, 90, Color(0, 0, 0, 255), 7);
            // Texture of the top of the head of the snake
            a_top_snake_dead_head_texture = std::make_shared<Image>(100, 100, Color(0, 0, 0, 0));
            a_top_snake_dead_head_texture.get()->draw_line(10, 10, 35, 35, Color(0, 0, 0, 255), 7);
            a_top_snake_dead_head_texture.get()->draw_line(35, 10, 10, 35, Color(0, 0, 0, 255), 7);
            a_top_snake_dead_head_texture.get()->draw_line(90, 10, 65, 35, Color(0, 0, 0, 255), 7);
            a_top_snake_dead_head_texture.get()->draw_line(65, 10, 90, 35, Color(0, 0, 0, 255), 7);

            // Start the game
            reset_game();
        }

        // Calculate the positions of the GUI
        void Snake::calculate_GUI_positions() {
            // Calculate the datas about the cases
            unsigned int current_pos = a_outer_line_width * 2;
            a_cases_height = partition_number(window_width() - (a_outer_line_width * 3 - a_inner_line_width), a_height_in_cases);
            a_cases_height[a_cases_height.size() - 1];
            a_cases_width = partition_number(window_width() - (a_outer_line_width * 3 - a_inner_line_width), a_width_in_cases);
            a_cases_width[a_cases_width.size() - 1];
            a_cases_x.clear();
            for(int i = 0;i<static_cast<int>(a_cases_width.size());i++) {
                a_cases_x.push_back(current_pos);
                current_pos += a_cases_width[i];
            }
            current_pos = a_outer_line_width * 2;
            a_cases_y.clear();
            for(int i = 0;i<static_cast<int>(a_cases_height.size());i++) {
                a_cases_y.push_back(current_pos);
                current_pos += a_cases_height[i];
            }
        }

        // Deletes an object in the game
        void Snake::delete_object(Snake_Object* object_to_delete) {
            for(int i = 0;i<static_cast<int>(a_objects.size());i++) {
                if(a_objects[i].get() == object_to_delete) {
                    a_objects.erase(a_objects.begin() + i);
                    a_playground->delete_child(object_to_delete);
                    return;
                }
            }
        }

        // Kill a snake
        void Snake::kill_snake(Snake_Total &snake) {
            if(a_lose_text != 0) a_lose_text->set_visible(true);
            if(a_restart_button != 0) a_restart_button->set_visible(true);
            snake.kill();
            update_score(snake);
        }

        // Move an object at a certain case
        void Snake::move_object(Snake_Object* piece, unsigned int case_x, unsigned int case_y) {
            piece->set_height_in_pixel(a_cases_height[case_y] - 1);
            piece->set_width_in_pixel(a_cases_width[case_x] - 1);
            piece->set_x_in_pixel(a_cases_x[case_x]);
            piece->set_y_in_pixel(a_cases_y[case_y]);
            piece->set_x_in_cases(case_x);
            piece->set_y_in_cases(case_y);
        }

        // Returns the object at a position in the game
        Snake_Object* Snake::object_at(unsigned short x, unsigned short y) {
            for(int i = 0;i<static_cast<int>(a_objects.size());i++) {
                if(a_objects[i]->x_in_cases() == x && a_objects[i]->y_in_cases() == y) {
                    return a_objects[i].get();
                }
            }
            for(int i = 0;i<static_cast<int>(a_snake_pieces.size());i++) {
                if(a_snake_pieces[i]->x_in_cases() == x && a_snake_pieces[i]->y_in_cases() == y) {
                    return a_snake_pieces[i].get();
                }
            }
            return 0;
        }

        // Create the background image of the playground part
        std::shared_ptr<Image> Snake::playground_image() {
            calculate_GUI_positions();
            std::shared_ptr<Image> to_return = std::make_shared<Image>(window_width(), window_width(), Color(160, 160, 160));

            // Draw the outer lines
            Image* image = to_return.get();
            image->fill_rect(0, 0, a_outer_line_width, image->height() - a_outer_line_width, Color(60, 60, 60));
            image->fill_rect(0, image->height() - a_outer_line_width, image->width(), a_outer_line_width, Color(60, 60, 60));
            image->fill_rect(a_outer_line_width, 0, a_outer_line_width, image->height() - a_outer_line_width * 2, Color(102, 102, 102));
            image->fill_rect(a_outer_line_width, image->height() - a_outer_line_width * 2, image->width() - a_outer_line_width, a_outer_line_width, Color(102, 102, 102));
            image->fill_rect(a_outer_line_width * 2, 0, image->width() - a_outer_line_width * 2, a_outer_line_width, Color(200, 200, 200));
            image->fill_rect(image->width() - a_outer_line_width, 0, a_outer_line_width, image->height() - a_outer_line_width * 2, Color(200, 200, 200));

            // Draw the inner lines
            for(int i = 0;i<static_cast<int>(a_cases_width.size());i++) {
                if(i < static_cast<int>(a_cases_width.size()) - 1) image->draw_line((a_cases_x[i] + a_cases_width[i]) - a_inner_line_width, a_outer_line_width - 1, (a_cases_x[i] + a_cases_width[i]) - a_inner_line_width, image->height() - a_outer_line_width * 2, scls::Color(102, 102, 102));
            }
            for(int i = 0;i<static_cast<int>(a_cases_height.size());i++) {
                if(i < static_cast<int>(a_cases_height.size()) - 1) { image->draw_line(a_outer_line_width - 1, image->height() - (a_cases_y[i] + a_cases_height[i]), (image->width() - a_outer_line_width) - 1, image->height() - (a_cases_y[i] + a_cases_height[i]), scls::Color(102, 102, 102)); }
            } //*/

            return to_return;
        }

        // Reset the game
        void Snake::reset_game() {
            a_loaded_snakes.clear();
            if(a_lose_text != 0) a_lose_text->set_visible(false);
            if(a_restart_button != 0) a_restart_button->set_visible(false);

            // Create a new snake
            a_loaded_snakes.push_back(std::make_unique<Snake_Total>(*this, "main"));
            update_score(*a_loaded_snakes[a_loaded_snakes.size() - 1].get());
        }

        // Update the Snake
        void Snake::update() {
            update_objects();
            update_snakes();
            update_snake_pieces();

            if(a_restart_button != 0 && a_restart_button->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                reset_game();
            }
        }

        // Update the objects
        void Snake::update_objects() {
            unsigned char object_number = 3;

            for(int i = 0;i<object_number - static_cast<int>(a_objects.size());i++) {
                unsigned short x_pos = 0;
                unsigned short y_pos = 0;
                do {
                    x_pos = rand()%a_width_in_cases;
                    y_pos = rand()%a_height_in_cases;
                } while(object_at(x_pos, y_pos) != 0);

                // Create the food object
                std::shared_ptr<Snake_Object> new_created_object = *a_playground->new_object<Snake_Object>("food_" + std::to_string(a_objects_created));
                new_created_object->set_background_color(Color(255, 0, 0));
                new_created_object->set_type("food");
                move_object(new_created_object.get(), x_pos, y_pos);
                a_objects_created++;
                a_objects.push_back(new_created_object);
            }
        }

        // Update the score according to a snake
        void Snake::update_score(Snake_Total &snake) {
            if(a_score != 0) {
                a_score->set_text("Score : " + std::to_string(snake.pieces_number() - 1) + " / " + std::to_string(a_width_in_cases * a_height_in_cases - 1));
            }
        }

        // Update the number of snake pieces
        void Snake::update_snake_pieces() {
            unsigned int total_pieces = 0;
            for(int i = 0;i<static_cast<int>(a_loaded_snakes.size());i++) {
                total_pieces += a_loaded_snakes[i]->pieces_number();
            }

            // Create the needed pieces
            for(int i = 0;i<static_cast<int>(total_pieces) - static_cast<int>(a_snake_pieces.size());i++) {
                std::shared_ptr<Snake_Piece> a_piece = *a_playground->new_object<Snake_Piece>("piece_" + std::to_string(a_pieces_created));
                move_object(a_piece.get(), 0, 0);
                a_pieces_created++;
                a_snake_pieces.push_back(a_piece);
            }

            // Remove the needed pieces
            while(static_cast<int>(a_snake_pieces.size()) > total_pieces) {
                a_playground->delete_child(a_snake_pieces[a_snake_pieces.size() - 1].get());
                a_snake_pieces.pop_back();
            }

            // Place the pieces
            total_pieces = 0;
            for(int i = 0;i<static_cast<int>(a_loaded_snakes.size());i++) {
                Snake_Total* current_snake = a_loaded_snakes[i].get();
                for(int i = 0;i<static_cast<int>(current_snake->pieces_number());i++) {
                    if(i >= static_cast<int>(current_snake->pieces_number()) - 1) {
                        if(current_snake->is_dead()) {
                            if(current_snake->last_move() == 0) a_current_snake_head_texture = a_top_snake_dead_head_texture;
                            else if(current_snake->last_move() == 1) a_current_snake_head_texture = a_left_snake_dead_head_texture;
                            else if(current_snake->last_move() == 2) a_current_snake_head_texture = a_bottom_snake_dead_head_texture;
                            else a_current_snake_head_texture = a_right_snake_dead_head_texture;
                        }
                        else {
                            if(current_snake->last_move() == 0) a_current_snake_head_texture = a_top_snake_head_texture;
                            else if(current_snake->last_move() == 1) a_current_snake_head_texture = a_left_snake_head_texture;
                            else if(current_snake->last_move() == 2) a_current_snake_head_texture = a_bottom_snake_head_texture;
                            else a_current_snake_head_texture = a_right_snake_head_texture;
                        }
                        a_snake_pieces[total_pieces]->texture()->set_image(a_current_snake_head_texture);
                    }
                    else {
                        a_snake_pieces[total_pieces]->texture()->set_image(0);
                    }
                    move_object(a_snake_pieces[total_pieces].get(), current_snake->pieces_x()[i], current_snake->pieces_y()[i]);
                    total_pieces++;
                }
            }
        }

        // Update the loaded snakes
        void Snake::update_snakes() {
            unsigned int total_pieces = 0;
            for(int i = 0;i<static_cast<int>(a_loaded_snakes.size());i++) {
                Snake_Total* current_snake = a_loaded_snakes[i].get();
                if(current_snake->last_move_time() > 1.0 / static_cast<double>(current_snake->speed())) {
                    if(!current_snake->is_dead()) {
                        // Active the cheat
                        if(key_state("p") == Key_State::Pressed) {
                            current_snake->set_use_cheat(true);
                        }
                        // If the snake cheat
                        if(current_snake->use_cheat()) {
                            if(current_snake->y() == 0) {
                                if(current_snake->x() == 0) current_snake->set_last_move(0);
                                else if(current_snake->x() == a_width_in_cases - 1) current_snake->set_last_move(1);
                                else if(current_snake->x() == 0) current_snake->set_last_move(1);
                            }
                            else if(current_snake->y() == 1) {
                                if(current_snake->x() == a_width_in_cases - 1) current_snake->set_last_move(2);
                                else if(current_snake->x() % 2 == 0) current_snake->set_last_move(0);
                                else current_snake->set_last_move(3);
                            }
                            else if(current_snake->y() >= a_height_in_cases - 1) {
                                if(current_snake->x() % 2 == 0) current_snake->set_last_move(3);
                                else current_snake->set_last_move(2);
                            }
                            else {
                                if(current_snake->x() % 2 == 0) current_snake->set_last_move(0);
                                else current_snake->set_last_move(2);
                            }
                        }

                        // Check the keyboard
                        if(key_state("left arrow") == Key_State::Pressed) {
                            current_snake->set_last_move(1);
                        }
                        if(key_state("right arrow") == Key_State::Pressed) {
                            current_snake->set_last_move(3);
                        }
                        if(key_state("up arrow") == Key_State::Pressed) {
                            current_snake->set_last_move(0);
                        }
                        if(key_state("down arrow") == Key_State::Pressed) {
                            current_snake->set_last_move(2);
                        }

                        // Move the snake
                        short new_x = 0;
                        short new_y = 0;
                        if(current_snake->last_move() == 0) {
                            new_x = current_snake->pieces_x()[current_snake->pieces_x().size() - 1];
                            new_y = current_snake->pieces_y()[current_snake->pieces_y().size() - 1] + 1;
                        }
                        else if(current_snake->last_move() == 1) {
                            new_x = current_snake->pieces_x()[current_snake->pieces_x().size() - 1] - 1;
                            new_y = current_snake->pieces_y()[current_snake->pieces_y().size() - 1];
                        }
                        else if(current_snake->last_move() == 2) {
                            new_x = current_snake->pieces_x()[current_snake->pieces_x().size() - 1];
                            new_y = current_snake->pieces_y()[current_snake->pieces_y().size() - 1] - 1;
                        }
                        else if(current_snake->last_move() == 3) {
                            new_x = current_snake->pieces_x()[current_snake->pieces_x().size() - 1] + 1;
                            new_y = current_snake->pieces_y()[current_snake->pieces_y().size() - 1];
                        }

                        // Reset the snake
                        if(current_snake->last_move() != -1) {
                            if(new_x >= 0 && new_y >= 0 && new_x < a_width_in_cases && new_y < a_height_in_cases) {
                                Snake_Object* current_object = object_at(new_x, new_y);
                                bool finaly_move_snake = true;
                                if(current_object == 0) {
                                    current_snake->pieces_x().erase(current_snake->pieces_x().begin());
                                    current_snake->pieces_y().erase(current_snake->pieces_y().begin());
                                }
                                else if(current_object->type() == "snake") {
                                    kill_snake(*current_snake);
                                    finaly_move_snake = false;
                                }
                                if(finaly_move_snake) {
                                    current_snake->pieces_x().push_back(new_x);
                                    current_snake->pieces_y().push_back(new_y);
                                }
                                if(current_object != 0 && current_object->type() == "food") {
                                    delete_object(current_object);
                                    update_score(*current_snake);
                                }
                                current_snake->reset_last_move_time();
                            }
                            else {
                                kill_snake(*current_snake);
                            }
                        }
                    }
                }
                else {
                    current_snake->set_last_move_time(current_snake->last_move_time() + delta_time());
                }
            }
        }

        // Use the Snake window
        void use_snake(int window_width, int window_height, std::string exec_path) {
            std::unique_ptr<Snake> snake = std::make_unique<Snake>(window_width, window_height, exec_path);

            // Execution loop
            while(snake.get()->run()) {
                snake.get()->update_event();
                snake.get()->update();

                snake.get()->render();
            }
        }
    }
}
