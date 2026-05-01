import sys

with open('src/Rooms.cpp', 'r') as f:
    text = f.read()

old_str = """    // Draw the ImGui riddle popup if active
    if (showRiddlePopup) {
        player.isControlDisabled = true; // freeze player
        
        if (riddleIsWriting && !riddleFinishedWriting) {
            // Update logic for typing the riddle character by character
            if (riddlePauseTimer > 0.f) {
                riddlePauseTimer -= deltaTime;
            } else if (riddleCharIndex < fullRiddleText.length()) {
                riddleCharTimer -= deltaTime;
                if (riddleCharTimer <= 0.f) {
                    char c = fullRiddleText[riddleCharIndex];
                    currentRiddleText += c;
                    riddleCharIndex++;
                    
                    if (c == '.' || c == '\\n') {
                        riddlePauseTimer = 500.f; // Add delay at the end of sentence/line
                    } else {
                        riddleCharTimer = 20.f; // normal delay between letters
                    }
                }
            } else {
                riddleFinishedWriting = true;
                riddleIsWriting = false;
            }
            
            riddleScreenText.setString(currentRiddleText);
        } else {
            // ImGui popup for actual riddle text input
            ImGui::SetNextWindowPos(ImVec2(1920.f / 2.f, 1080.f / 2.f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(800.f, 300.f), ImGuiCond_Appearing);
            
            ImGui::Begin("Table Riddle", &showRiddlePopup, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
            
            ImGui::SetWindowFontScale(1.5f); // Scale text inside safely
            
            if (riddlesCompleted) {
                ImGui::TextWrapped("Alas, thou hast conquered the trial of wits and bested the table's riddles!");
                ImGui::TextWrapped("\\nThy code of passage is: ROGBY");
                if (ImGui::Button("Close", ImVec2(120, 40))) {
                    showRiddlePopup = false;
                    player.isControlDisabled = false;
                }
            } else {
                ImGui::TextWrapped("What is the answer to the colored doors?");
                
                static char riddleInput[16] = "";  // Need this to persist across frames
                static bool setFocus = false;
                
                // Auto-focus the input box down so the user doesn't have to click
                if (ImGui::IsWindowAppearing() || setFocus) {
                    ImGui::SetKeyboardFocusHere();
                    setFocus = false;
                }

                if (ImGui::InputText("##RiddleAnswer", riddleInput, 16, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsUppercase)) {
                    if (std::string(riddleInput) == "ROGBY") {
                        std::cout << "Correct!\\n";
                        audioManager.playInteractSound();
                        riddleInput[0] = '\\0';
                        setFocus = true;
                        riddlesCompleted = true;
                    } else {
                        std::cout << "Incorrect Answer!\\n";
                        riddleInput[0] = '\\0';
                        setFocus = true;
                        audioManager.playDeniedSound();
                    }
                }
                
                if (ImGui::Button("Submit", ImVec2(120, 40))) {
                    if (std::string(riddleInput) == "ROGBY") {
                        std::cout << "Correct!\\n";
                        audioManager.playInteractSound();
                        riddleInput[0] = '\\0';
                        setFocus = true;
                        riddlesCompleted = true;
                    } else {
                        std::cout << "Incorrect Answer!\\n";
                        riddleInput[0] = '\\0';
                        setFocus = true;
                        audioManager.playDeniedSound();
                    }
                }
                
                ImGui::SameLine();
                
                if (ImGui::Button("Cancel", ImVec2(120, 40))) {
                    showRiddlePopup = false;
                    player.isControlDisabled = false;
                    riddleInput[0] = '\\0';
                }
            }
            
            // Re-enable player if closed manually via "X" button
            if (!showRiddlePopup) {
                player.isControlDisabled = false;
            }

            ImGui::End();
        }
    }"""

new_str = """    // Draw the ImGui riddle popup if active
    if (showRiddlePopup) {
        player.isControlDisabled = true; // freeze player
        
        struct Question {
            std::string text;
            std::string correctAnswer;
        };
        
        static std::vector<Question> gameStage = {
            {"How many ducks are in this scenario: 2 ducks in front of a duck, 2 ducks behind a duck, and 1 duck in the middle?", "3"},
            {"If you are running a race and you pass the person in second place, what place are you in?", "2"},
            {"If A = 1, B = 2, C = 3, etc., what is the sum of 'BAD'?", "7"},
            {"If you rotate the number 6 180 degrees, what number do you get?", "9"}
        };
        
        // Center the window on screen
        ImGui::SetNextWindowPos(ImVec2(1920.f / 2.f, 1080.f / 2.f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(800.f, 300.f), ImGuiCond_Appearing);
        
        ImGui::Begin("Table Riddle", &showRiddlePopup, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        
        ImGui::SetWindowFontScale(1.5f); // Scale text inside safely
        
        if (riddlesCompleted) {
            ImGui::TextWrapped("Alas, thou hast conquered the trial of wits and bested the table's riddles!");
            ImGui::TextWrapped("\\nThy code of passage is: 3279");
            if (ImGui::Button("Close", ImVec2(120, 40))) {
                showRiddlePopup = false;
                player.isControlDisabled = false;
            }
        } else {
            ImGui::TextWrapped("Question %d/4:", currentRiddle + 1);
            ImGui::TextWrapped("%s", gameStage[currentRiddle].text.c_str());
            
            static char riddleInput[16] = "";  // Need this to persist across frames
            static bool setFocus = false;
            
            // Auto-focus the input box down so the user doesn't have to click
            if (ImGui::IsWindowAppearing() || setFocus) {
                ImGui::SetKeyboardFocusHere();
                setFocus = false;
            }

            if (ImGui::InputText("##RiddleAnswer", riddleInput, 16, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal)) {
                if (std::string(riddleInput) == gameStage[currentRiddle].correctAnswer) {
                    std::cout << "Correct!\\n";
                    audioManager.playInteractSound();
                    currentRiddle++;
                    riddleInput[0] = '\\0';
                    setFocus = true;
                    
                    if (currentRiddle >= gameStage.size()) {
                        riddlesCompleted = true;
                        std::cout << "All riddles solved!\\n";
                    }
                } else {
                    std::cout << "Incorrect Answer!\\n";
                    riddleInput[0] = '\\0';
                    setFocus = true;
                    audioManager.playDeniedSound();
                }
            }
            
            if (ImGui::Button("Submit", ImVec2(120, 40))) {
                if (std::string(riddleInput) == gameStage[currentRiddle].correctAnswer) {
                    std::cout << "Correct!\\n";
                    audioManager.playInteractSound();
                    currentRiddle++;
                    riddleInput[0] = '\\0';
                    setFocus = true;
                    
                    if (currentRiddle >= gameStage.size()) {
                        riddlesCompleted = true;
                        std::cout << "All riddles solved!\\n";
                    }
                } else {
                    std::cout << "Incorrect Answer!\\n";
                    riddleInput[0] = '\\0';
                    setFocus = true;
                    audioManager.playDeniedSound();
                }
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Cancel", ImVec2(120, 40))) {
                showRiddlePopup = false;
                player.isControlDisabled = false;
                riddleInput[0] = '\\0';
            }
        }
        
        // Re-enable player if closed manually via "X" button
        if (!showRiddlePopup) {
            player.isControlDisabled = false;
        }

        ImGui::End();
    }

    // Draw the ImGui ROGBY popup if active
    if (showROGBYPopup) {
        player.isControlDisabled = true; // freeze player
        
        if (riddleIsWriting && !riddleFinishedWriting) {
            // Update logic for typing the riddle character by character
            if (riddlePauseTimer > 0.f) {
                riddlePauseTimer -= deltaTime;
            } else if (riddleCharIndex < fullRiddleText.length()) {
                riddleCharTimer -= deltaTime;
                if (riddleCharTimer <= 0.f) {
                    char c = fullRiddleText[riddleCharIndex];
                    currentRiddleText += c;
                    riddleCharIndex++;
                    
                    if (c == '.' || c == '\\n') {
                        riddlePauseTimer = 500.f; // Add delay at the end of sentence/line
                    } else {
                        riddleCharTimer = 20.f; // normal delay between letters
                    }
                }
            } else {
                riddleFinishedWriting = true;
                riddleIsWriting = false;
            }
            
            riddleScreenText.setString(currentRiddleText);
        } else {
            // ImGui popup for actual riddle text input
            ImGui::SetNextWindowPos(ImVec2(1920.f / 2.f, 1080.f / 2.f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(800.f, 300.f), ImGuiCond_Appearing);
            
            ImGui::Begin("Table Riddle", &showROGBYPopup, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
            
            ImGui::SetWindowFontScale(1.5f); // Scale text inside safely
            
            if (rogbyCompleted) {
                ImGui::TextWrapped("Alas, thou hast conquered the trial of wits and bested the table's riddles!");
                ImGui::TextWrapped("\\nThy code of passage is: ROGBY");
                if (ImGui::Button("Close", ImVec2(120, 40))) {
                    showROGBYPopup = false;
                    player.isControlDisabled = false;
                }
            } else {
                ImGui::TextWrapped("What is the answer to the colored doors?");
                
                static char rogbyInput[16] = "";  // Need this to persist across frames
                static bool setFocus = false;
                
                // Auto-focus the input box down so the user doesn't have to click
                if (ImGui::IsWindowAppearing() || setFocus) {
                    ImGui::SetKeyboardFocusHere();
                    setFocus = false;
                }

                if (ImGui::InputText("##ROGBYAnswer", rogbyInput, 16, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsUppercase)) {
                    if (std::string(rogbyInput) == "ROGBY") {
                        std::cout << "Correct!\\n";
                        audioManager.playInteractSound();
                        rogbyInput[0] = '\\0';
                        setFocus = true;
                        rogbyCompleted = true;
                    } else {
                        std::cout << "Incorrect Answer!\\n";
                        rogbyInput[0] = '\\0';
                        setFocus = true;
                        audioManager.playDeniedSound();
                    }
                }
                
                if (ImGui::Button("Submit", ImVec2(120, 40))) {
                    if (std::string(rogbyInput) == "ROGBY") {
                        std::cout << "Correct!\\n";
                        audioManager.playInteractSound();
                        rogbyInput[0] = '\\0';
                        setFocus = true;
                        rogbyCompleted = true;
                    } else {
                        std::cout << "Incorrect Answer!\\n";
                        rogbyInput[0] = '\\0';
                        setFocus = true;
                        audioManager.playDeniedSound();
                    }
                }
                
                ImGui::SameLine();
                
                if (ImGui::Button("Cancel", ImVec2(120, 40))) {
                    showROGBYPopup = false;
                    player.isControlDisabled = false;
                    rogbyInput[0] = '\\0';
                }
            }
            
            // Re-enable player if closed manually via "X" button
            if (!showROGBYPopup) {
                player.isControlDisabled = false;
            }

            ImGui::End();
        }
    }"""

if old_str in text:
    with open('src/Rooms.cpp', 'w') as f:
        f.write(text.replace(old_str, new_str))
    print("Success")
else:
    print("Fail: String not found")
