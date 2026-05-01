with open("src/Rooms.cpp", "r") as f:
    lines = f.readlines()

new_lines = []
skip_next = False
for i, line in enumerate(lines):
    if skip_next:
        skip_next = False
        continue
    if "if (c == '.' || c == '\\n'" in line:
        new_lines.append("                        if (c == '.' || c == '\\n') {\n")
        skip_next = True
    else:
        new_lines.append(line)

with open("src/Rooms.cpp", "w") as f:
    f.writelines(new_lines)
