#include "App.h"
#include "settings.h"
#include <bitset>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#define PI 3.14159265358979
#define TWO_PI PI*2
#define HALF_PI PI/2
#define THREE_PI_OVER_TWO 3*PI/2
#define RD 0.01745329

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
                                                
ImVec2 operator+(const ImVec2 & l,const ImVec2 & r) {   
    return {l.x+r.x,l.y+r.y};                                    
}
                                                
ImVec2 operator-(const ImVec2& l,const ImVec2 & r) {   
    return {l.x-r.x,l.y-r.y};                                    
}
App::App(const char* title, int width, int height) {
    caster = new RayCaster(maze);
    window_title = title;
    window_width = width;
    window_height = height;
    maze = new Maze(MAZE_WIDTH, MAZE_HEIGHT);

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(1);

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    window = glfwCreateWindow(width, height, window_title, NULL, NULL);
    if (window == NULL)
        exit(1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    show_info = true;
    show_maze = true;
    show_controls = true;
    show_demo = false;
    show_menubar = true;
    show_raycast = true;
    steps_gen = 0;
    steps_solve = 0;
    len_path = 0;
    cell_to_wall = 1.0f;
    timescale = 0.0f;

    
}

void App::run() {
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        // if (show_menubar)
        //     renderMenuBar();

        if (show_controls)
            renderControls();

        if (show_info) 
            renderInfo();
    
        if (show_demo)
            ImGui::ShowDemoWindow(&show_demo);

        if (show_maze)
            renderMaze();

        if (show_raycast)
            renderRayCast();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
}

App::~App() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    free(maze);
}

void App::renderControls() {
    ImGui::Begin("Controls", &show_controls);
    if (ImGui::Button("Reset")) {
        resetMaze();
    }

    static const char* gen_algos[] = {"DFS", "Kruskal", "Prim's"};
    static int current_gen_algo = 0;

    static const char* solve_algos[] = {"DFS", "BFS", "Dijkstra", "A*"};
    static int current_solve_algo = 0;
    
    static int maze_dimensions[2];
    maze_dimensions[0] = maze->getWidth();
    maze_dimensions[1] = maze->getHeight();

    if (ImGui::InputInt2("Maze Dimensions", maze_dimensions) && maze_dimensions[0] > 0 && maze_dimensions[1] > 0) {
        resetMaze();
        maze->resize(maze_dimensions[0], maze_dimensions[1]);
    }
    ImGui::Checkbox("Animate", &maze->isAsync);
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.25f);
    ImGui::Combo("Generation Algorithm", &current_gen_algo, gen_algos, IM_ARRAYSIZE(gen_algos));

    ImGui::SameLine();
    if (ImGui::Button("Generate") && !maze_generating) {
        maze_generating = true;
        
        resetMaze();
        std::string func = gen_algos[current_gen_algo];
        maze->generate(func, this->steps_gen);
        //if (maze->isGenerated()) t.join();
        maze_generating = false;
    }
    if (!maze->isGenerated())
        ImGui::BeginDisabled();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.25f);
    ImGui::Combo("Solve Algorithm", &current_solve_algo, solve_algos, IM_ARRAYSIZE(solve_algos));
        
    ImGui::SameLine();
    if (ImGui::Button("Solve") && !maze_solving) {
        maze_solving = true;
        if (maze->isSolved()) {
            maze->unsolve();
            steps_solve = 0;
            len_path = 0;
        }
        // if animate solving send to thread!
        maze->unsolve();
        std::string func = solve_algos[current_solve_algo];
        maze->solve(func, this->steps_solve, this->len_path);
        maze_solving = false;
    }
    if (!maze->isGenerated())
        ImGui::EndDisabled();
    
    if (ImGui::SliderFloat("Animation Speed", &timescale, -1.0f, 1.0f)) {
        maze->delay = DEFAULT_DELAY - timescale*DEFAULT_DELAY;
    }

    ImGui::End();
}

ImU32 App::getFillCol(mnode* m) {
    if (m == maze->activeNode) return active_col;
    else if (MNODE_FINISH(*m)) return finish_col;
    else if (MNODE_START(*m)) return start_col;
    else if (MNODE_PATH(*m)) return path_col;
    else if (MNODE_WASTED(*m)) return wasted_col;
    else return bg_col;
}

void App::renderMaze() {
    ImGui::Begin("Maze");
    static ImVec2 pan(0.0f, 0.0f);
    static float zoom = DEFAULT_ZOOM;
    ImGui::SliderFloat("Cell to wall ratio", &cell_to_wall, 0.5f, 10.0f);
    ImGui::SliderFloat("Zoom", &zoom, 1.0f, 10.0f);
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held
    // Pan (we use a zero mouse threshold when there's no context menu)
    // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
    const float mouse_threshold_for_pan = -1.0f;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
    {
        pan.x += io.MouseDelta.x;
        pan.y += io.MouseDelta.y;
    }
    // Context menu (under default mouse threshold)
    ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    if (drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
    if (ImGui::BeginPopup("context"))
    {
        if (ImGui::MenuItem("Reset view", NULL, false, true)) { pan.x = 0; pan.y = 0; zoom = DEFAULT_ZOOM; }
        if (ImGui::MenuItem("Save as PNG", NULL, false, true)) { savePNG(); }
        ImGui::EndPopup();
    }
    
    ImVec2 origin(canvas_p0.x + pan.x, canvas_p0.y + pan.y); // Lock scrolled origin
    ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
    ImVec2 wall_sz = {canvas_sz.x / maze->getWidth()/(cell_to_wall*2), canvas_sz.y / maze->getHeight()/(cell_to_wall*2)};
    ImVec2 cell_sz = {canvas_sz.x / maze->getWidth() - wall_sz.x, canvas_sz.y / maze->getHeight() - wall_sz.y};
    ImVec2 full_sz = cell_sz + wall_sz;
    if (io.KeyShift) {
        origin = canvas_p0 - ImVec2(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y) + pan;
        wall_sz = ImVec2(wall_sz.x * zoom, wall_sz.y * zoom);
        cell_sz = ImVec2(cell_sz.x * zoom, cell_sz.y * zoom);
        full_sz = cell_sz + wall_sz;
    }
    else if (io.KeyAlt) {
        wall_sz = ImVec2(wall_sz.x * zoom, wall_sz.y * zoom);
        cell_sz = ImVec2(cell_sz.x * zoom, cell_sz.y * zoom);
        full_sz = cell_sz + wall_sz;
        coord c = maze->getCoord(maze->activeNode);
        ImVec2 active_pos = ImVec2(c.first*full_sz.x, c.second*full_sz.y);
        origin = canvas_p0 - active_pos + ImVec2(canvas_sz.x/2, canvas_sz.y/2);
    }
    // draw grid
    draw_list->AddRectFilled(canvas_p0, canvas_p1, wall_col);
    uint minx = std::max(0,(int)((canvas_p0.x - origin.x)/full_sz.x));
    uint miny = std::max(0,(int)((canvas_p0.y - origin.y)/full_sz.y));
    uint maxx = std::max(0,std::min((int)maze->getWidth(),(int)ceil((canvas_p1.x - origin.x)/full_sz.x)));
    uint maxy = std::max(0,std::min((int)maze->getHeight(),(int)ceil((canvas_p1.y - origin.y)/full_sz.y)));
    for (uint i = minx; i < maxx; ++i) {
        for (uint j = miny; j < maxy; ++j) {
            ImVec2 p0 = {full_sz.x * i + origin.x, full_sz.y * j + origin.y};
            ImVec2 p1 = p0 + wall_sz + cell_sz + wall_sz;
            mnode* cur = maze->getNode(i,j);
            draw_list->AddRectFilled(p0+wall_sz, p0+wall_sz+cell_sz, getFillCol(cur));
            // draw walls
            if (!MNODE_GET_WALL(*cur, 0)) {
                draw_list->AddRectFilled(ImVec2(p0.x + wall_sz.x, p0.y), ImVec2(p1.x - wall_sz.x, p0.y + wall_sz.y), getFillCol(cur));
            }
            if (!MNODE_GET_WALL(*cur, 1)) {
                draw_list->AddRectFilled(ImVec2(p1.x - wall_sz.x, p0.y + wall_sz.y), ImVec2(p1.x, p1.y - wall_sz.y), getFillCol(cur));
            }
            if (!MNODE_GET_WALL(*cur, 2)) {
                draw_list->AddRectFilled(ImVec2(p0.x + wall_sz.x, p1.y - wall_sz.y), ImVec2(p1.x - wall_sz.x, p1.y), getFillCol(cur));
            }
            if (!MNODE_GET_WALL(*cur, 3)) {
                draw_list->AddRectFilled(ImVec2(p0.x, p1.y - wall_sz.y), ImVec2(p0.x + cell_sz.x, p1.y - wall_sz.y), getFillCol(cur));
            }
        }
    }
    ImGui::End();
}

void App::renderInfo() {
    ImGui::Begin("Info", &show_info);
    ImGui::SetWindowFontScale(1.8);
    ImGui::Text("Mazer by Ryan Hellwege");
    ImGui::SetWindowFontScale(1.3);
    ImGui::Text("Controls");
    ImGui::SetWindowFontScale(1.0);
    ImGui::Text("ALT: follow the actiive node\nSHIFT: zoom into cursor\nCTRL + DRAG: Change the position of the start or finish");
    ImGui::Separator();
    ImGui::SetWindowFontScale(1.3);
    ImGui::Text("Output");
    ImGui::SetWindowFontScale(1.0);
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::Text("Maze Dimensions: (%.i, %.i)", maze->getWidth(), maze->getHeight());
    ImGui::Text("Seed %.u", maze->getSeed());
    if (!maze->isGenerated())
        ImGui::Text("Maze State: EMPTY");
    else if (!maze->isSolved())
        ImGui::Text("Maze State: UNSOLVED");
    else
        ImGui::Text("Maze State: SOLVED!");
    ImGui::Text("Generation Progress: %.i/%.i", steps_gen, maze->getArea()-1);
    if (maze->isGenerated()) {
        ImGui::Text("Solve steps: %.i", steps_solve);
        ImGui::Text("Path length: %.i", len_path);
    }
    ImGui::End();
}

void App::renderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Export Maze")) {
                std::cout << "Exporting maze..." << std::endl;
            }
            if (ImGui::MenuItem("Import Maze")) {
                std::cout << "Exporting maze..." << std::endl;
            }
        }
    }
}

inline void App::resetMaze() {
    maze->reset();
    steps_gen = 0;
    steps_solve = 0;
    len_path = 0;
}

void App::savePNG() {
    std::cout << "saving PNG..." << std::endl;
}

ImU32 App::getCastCol(float dist) {
}

void App::renderRayCast() {
    ImGui::Begin("Ray Cast");

    static float fov = 90.0f;
    static float angle = 0.0f;

    ImGui::SliderFloat("FOV", &fov, 60.0f, 180.0f);
    ImGui::SliderFloat("Angle", &angle, 0.0f, 360.0f);

    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held
    ImVec2 mouse_pos_in_canvas(io.MousePos.x - canvas_p0.x, io.MousePos.y - canvas_p0.y);
    // draw grid
    draw_list->AddRectFilled(canvas_p0, canvas_p1, wall_col);
    
    float rfov = RD*fov;
    float delta = rfov / (canvas_sz.x / RAYCAST_WIDTH);
    for (uint i = 0; i < canvas_sz.x; ++i) {
        
        
    }
    ImGui::End();
}