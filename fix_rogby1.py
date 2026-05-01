import re

with open("src/Rooms.cpp", "r") as f:
    text = f.read()

old_block = r"""    if \(showROGBYPopup\) \{
        player\.isControlDisabled = true; // freeze player
        
        if \(riddleIsWriting && !riddleFinishedWriting\) \{
            // Update logic for typing the riddle character by character
            if \(riddlePauseTimer > 0\.f\) \{
                riddlePauseTimer -= deltaTime;
            \} else if \(riddleCharIndex < fullRiddleText\.length\(\)\) \{
                riddleCharTimer -= deltaTime;
                if \(riddleCharTimer <= 0\.f\) \{
                    char c = fullRiddleText\[riddleCharIndex\];
                    currentRiddleText \+= c;
                    riddleCharIndex\+\+;
                    
                    if \(c == '\.' \|\| c == '\\n'\) \{
                        riddlePauseTimer = 500\.f; // Add delay at the end of sentence/line
                    \} else \{
                        riddleCharTimer = 20\.f; // normal delay between letters
                    \}
                \}
            \} else \{
                riddleFinishedWriting = true;
                riddleIsWriting = false;
            \}
            
            riddleScreenText\.setString\(currentRiddleText\);
        \} else \{
            // ImGui popup for actual riddle text input"""

new_block = """    if (showROGBYPopup) {
        player.isControlDisabled = true; // freeze player
        
        if (riddleIsWriting) {
            // Screen darken fade logic
            if (riddleDarkenAlpha < 220.f) {
                riddleDarkenAlpha += deltaTime * 0.2f;
                if (riddleDarkenAlpha > 220.f) riddleDarkenAlpha = 220.f;
            }
            
            if (!riddleFinishedWriting) {
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
                }
                riddleScreenText.setString(currentRiddleText);
            } else {
                // Show choice buttons AFTER finishing typing
                ImGui::SetNextWindowPos(ImVec2(1920.f / 2.f, 1080.f - 200.f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                ImGui::Begin("RiddleAction", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
                ImGui::SetWindowFontScale(1.8f);
                if (ImGui::Button("Enter Code", ImVec2(200, 60))) {
                    riddleIsWriting = false; // Transition to normal prompt
                }
                ImGui::SameLine(0, 50.f);
                if (ImGui::Button("Exit", ImVec2(200, 60))) {
                    showROGBYPopup = false;
                    player.isControlDisabled = false;
                }
                ImGui::End();
            }
        } else {
            // ImGui popup for actual riddle text input"""

text = re.sub(old_block, new_block, text, count=1, flags=re.DOTALL)

with open("src/Rooms.cpp", "w") as f:
    f.write(text)
print("Updated Rooms.cpp rewrite!")
