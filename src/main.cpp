#include "main.h"
#include "timer.h"
#include "ball.h"
#include "rectangle.h"
#include "semi.h"
#include "triangle.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

int score = 0;
Semi pool;
Semi magnetin,magnetout;
Ball  ball[100000];
Rectangle slope[1000];
Ball player;
int player_state = 0,in_water = 0;
double acc = 0.002f, upspeed = -0.15f,waterspeed = -0.1f;
float ball_rad_start = 0.2f, ball_rad_end = 0.6f;
float ball_var_start = 1.0f, ball_var_end = 4.5f;
float ball_x_start = -5.7f, ball_x_end = -5.0f;
float ball_vel_start = 0.01f, ball_vel_end = 0.05f;
float player_radius = 0.7f,player_x = -4.0f, player_y = -1.3f;
float pool_x = 0,pool_y = -2.0f,pool_radius = 1.5;
float tramp_x = 4,tramp_y=-1.65,tramp_w = 1.5,tramp_h = 0.7;
float vel = 0.05;
float spikes_x = -3.0f,spikes_dist = 0.4f,spikes_y=-2.0f;
int flag = 0;
int magnet_present = 0;
Triangle spikes[3];
Rectangle flore;
Rectangle tramp;
Semi tramp_curve;
Rectangle slopes;
int num = 8,numslopes = 3;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
extern double drag_oldx, drag_oldy;
int count = 0,spike_count=0,spike_flag=1;
char outsc[1000000];

//glfwindowsettitle
Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */

float random(float a,float b)
{
   return (a + (((float) rand()) / (float) RAND_MAX)*(b-a));
}

void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    if(magnet_present)
    {
        magnetin.draw(VP);
        magnetout.draw(VP);
    }
    for(int i=0;i<num;i++)
    {
        ball[i].draw(VP);
        if(i < numslopes)
            slope[i].draw(VP);
    }


    flore.draw(VP);
    pool.draw(VP);
    tramp.draw(VP);
    tramp_curve.draw(VP);
    spikes[0].draw(VP);
    spikes[1].draw(VP);
    spikes[2].draw(VP);
    player.draw(VP);

}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int A_left  = glfwGetKey(window, GLFW_KEY_A);

    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int D_right = glfwGetKey(window, GLFW_KEY_D);

    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int W_up = glfwGetKey(window, GLFW_KEY_W);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int SPACE_up = glfwGetKey(window, GLFW_KEY_SPACE);
    int mouse_clicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

//    int down = glfwGetKey(window, GLFW_KEY_DOWN);

    if ( D_right && player.position.x < 5 - player_radius && !detect_sides_tramp(player.bounding_box(),tramp.bounding_box())) {

//        if(player.position.x >= screen_center_x + 2.5 - player_radius)
//        {
//            screen_center_x+=0.05;
//            reset_screen();
//        }
        if(in_water && detect_collision_floor(player.bounding_box(),flore.bounding_box(),pool.bounding_box()))
        {
            if(player.position.x + 0.03f <= -player.radius + pool.radius)
            {
                player.set_position(player.position.x + 0.03f,player.position.y);
                player.set_position(player.position.x,pool_y - sqrt(abs((pool_radius - player_radius)*(pool_radius - player_radius) - player.position.x*player.position.x)));
            }
            if(player.position.x  > -player.radius + pool.radius)
            {
                player.set_speed(0,waterspeed);
                player_state = 1;
                flag = 1;
            }

        }
        else
            player.move(vel,0);

        // ball1.rotation+= 10;
    }
    if (A_left && player.position.x > player_radius - 5)
    {
//        if( player.position.x <= player_radius - 5){
//            screen_center_x-=0.1;
//            reset_screen();
//        }
        if(in_water && detect_collision_floor(player.bounding_box(),flore.bounding_box(),pool.bounding_box()))
        {
            if(player.position.x - 0.03f >= player.radius - pool.radius)
            {
                player.set_position(player.position.x - 0.03f,player.position.y);
    //            cout << "YO" << endl;
                player.set_position(player.position.x,pool_y - sqrt(abs((pool_radius - player_radius)*(pool_radius - player_radius) - player.position.x*player.position.x)));
            }
            if(player.position.x  < player.radius - pool.radius)
            {
                player.set_speed(0,waterspeed);
                player_state = 1;
                flag = 1;
            }
        }
        else
            player.move(-vel,0);
    }
    if ((SPACE_up || W_up)  && player.position.y < 5 && player_state == 0) {
        if(in_water == 1)
            player.set_speed(0,waterspeed);
        else
            player.set_speed(0,upspeed);

        player_state = 1;
    }
//    if (down && player.position.y > -0.8) {
//        player.move(0,-vel);
//    }

    if (left && screen_center_x - 4/screen_zoom > -4)
    {
        screen_center_x -= 0.1;
        reset_screen();
    }
    if (down && screen_center_y - 4/screen_zoom > -4)
    {
        screen_center_y -= 0.1;
        reset_screen();
    }
    if (right && screen_center_x + 4/screen_zoom < 4)
    {
        screen_center_x += 0.1;
        reset_screen();
    }
    if (up && screen_center_y + 4/screen_zoom < 4)
    {
        screen_center_y += 0.1;
        reset_screen();
    }

    if (mouse_clicked) {
        if (drag_oldx == -1 && drag_oldy == -1) {
            glfwGetCursorPos(window, &drag_oldx, &drag_oldy);
        }
        else {
            int w, h;
            double new_x, new_y;
            glfwGetCursorPos(window, &new_x, &new_y);
            glfwGetWindowSize(window, &w, &h);
            float pos_x, pos_y;
            pos_x = 8 * (new_x - drag_oldx) / (w * screen_zoom);
            player.position.x = pos_x;
            // drag_oldx = new_x;
            // drag_oldy = new_y;
        }
    }
}

void tick_elements()
{
//    cout << player_state << endl;

    count++;
    if(spikes[0].position.x > 90)
        spike_count++;

    if(spike_count == 500)
    {
        spikes[0].set_position(spikes_x,spikes_y);
        spikes[1].set_position(spikes_x - spikes_dist,spikes_y);
        spikes[2].set_position(spikes_x-2*spikes_dist,spikes_y);
        spike_flag = 1;
    }
    if(count == 400)
    {
        if(magnet_present == 1)
            magnet_present = 0;
        else
            magnet_present = 1;

        count = 0;
    }

    player.tick();
    if(magnet_present && player.position.y < 2.7 && player.position.y > 1.3)
            player.set_speed(player.speedx+0.001,player.speedy);

    spikes[0].tick();
    spikes[1].tick();
    spikes[2].tick();
    spikes_x-=spikes[0].speed;

    if(spikes_x < -4.0f)
    {
        spikes[0].speed = -0.01;
        spikes[1].speed = -0.01;
        spikes[2].speed = -0.01;
    }
    if(spikes_x > -2.5f)
    {
        spikes[0].speed = 0.01;
        spikes[1].speed = 0.01;
        spikes[2].speed = 0.01;
    }

    if(detect_spikes_surface(player.bounding_box()))
    {
        spikes[0].set_position(100,100);
        spikes[1].set_position(100,100);
        spikes[2].set_position(100,100);
        spike_count = 0;
        spike_flag = 0;
        score-=30;
    }
    if(detect_collision_tramp(player.bounding_box(),tramp.bounding_box()))
    {
        player_state = 1;
        player.set_speed(0,-player.speedy-0.05f);
    }
    if(detect_pool_surface(player.bounding_box(),pool.bounding_box()))
    {
        if(in_water == 0)
        {
            in_water = 1;
            player_state = 1;
            player.set_speed(0,0.05);
            if(player.position.x > pool_x + pool_radius - player_radius) //boundary cases
                player.set_position(pool_radius - player_radius,pool_y);
            if(player.position.x < pool_x - pool_radius + player_radius)
                player.set_position( -pool_radius + player_radius,pool_y);
        }
    }
    else
        in_water = 0;
    if(detect_collision_floor(player.bounding_box(),flore.bounding_box(),pool.bounding_box()))
    {
        if(!in_water){
        player.set_position(player.position.x,player_y);
        player.speedy = 0;
        player.speedx = 0;
        player_state = 0;
        }
        else
        {
            if(!flag)
            {
                player_state = 0;
            }
            else
                flag = 0;
            if(player.position.x <= 0){
                player.set_position(player.position.x + 0.02f,player.position.y);
                player.set_position(player.position.x,pool_y - sqrt(abs((pool_radius - player_radius)*(pool_radius - player_radius) - player.position.x*player.position.x)));
            }
            if(player.position.x > 0){
                player.set_position(player.position.x - 0.02f,player.position.y);
                player.set_position(player.position.x,pool_y - sqrt(abs((pool_radius - player_radius)*(pool_radius - player_radius) - player.position.x*player.position.x)));
            }

        }

    }
    if(player_state == 1)
    {
        player.set_speed(player.speedx,player.speedy+ acc);
    }

    for(int i=0;i<num;i++)
    {
        if(i < numslopes && player.speedy > 0 && detect_slope_collision(player.bounding_box(),slope[i].bounding_box(),slope[i].rotation))
        {
//             player.set_speed(-player.speedx,-player.speedy - 0.005f);
//            cout << "YO" << i << endl;
            float v = 0.05f;
            float a = slope[i].rotation;

            player.set_speed(-v*sin(a*M_PI/180),v*cos(a*M_PI/180));
             player.tick();

             float vary = random(ball_var_start,ball_var_end);
             float vely = random(ball_vel_start,ball_vel_end);
             float xy = random(ball_x_start,ball_x_end);

             ball[i].set_position(xy,vary);
             ball[i].set_speed(-vely,0);
             score+=ball[i].score;
             if(i<numslopes)
             {
                 float ang = slope[i].rotation;

                 slope[i].set_position(xy+ball[i].radius*cos(ang*M_PI/180 - M_PI/2),vary+ball[i].radius*sin(ang*M_PI/180 - M_PI/2));
                 slope[i].set_speed(-vely);
             }
        }
        if(player.speedy > 0 && detect_collision(player.bounding_box(),ball[i].bounding_box()))
        {
             player.set_speed(-player.speedx,-player.speedy - 0.005f);
             player.tick();

             float vary = random(ball_var_start,ball_var_end);
             float vely = random(ball_vel_start,ball_vel_end);
             float xy = random(ball_x_start,ball_x_end);

             ball[i].set_position(xy,vary);
             ball[i].set_speed(-vely,0);
             score+=ball[i].score;
             if(i<numslopes)
             {
                 slope[i].set_position(xy-ball[i].radius*cos((3*M_PI)/4),vary+ball[i].radius*sin((3*M_PI)/4));
                 slope[i].set_speed(-vely);
             }
        }

        ball[i].tick();
        slope[i].tick();
        if(ball[i].position.x > 5.1 + player_radius )
        {
            float vary = random(ball_var_start,ball_var_end);
            float vely = random(ball_vel_start,ball_vel_end);
            float rady = random(ball_rad_start,ball_rad_end);
            float xy = random(ball_x_start,ball_x_end);
            rady = 1;
            ball[i].set_position(xy,vary);
            ball[i].set_speed(-vely,0);
            if(i<numslopes)
            {
                float ang = slope[i].rotation;

                slope[i].set_position(xy+ball[i].radius*cos(ang*M_PI/180 - M_PI/2),vary+ball[i].radius*sin(ang*M_PI/180 - M_PI/2));
                slope[i].set_speed(-vely);
            }
        }
    }

    sprintf(outsc,"Current Score: %d", score);
    glfwSetWindowTitle(window,outsc);

    }

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    magnetin =Semi(-4,2,0.7,90,COLOR_BLACK);
    magnetout = Semi(-4,2,0.4,90,COLOR_BACKGROUND);
    pool = Semi(pool_x,pool_y,pool_radius,180,COLOR_BLUE);

    player = Ball(player_x,player_y,player_radius,0,0,0,COLOR_GREEN);

    tramp = Rectangle(tramp_x,tramp_y,tramp_w,tramp_h,0,0,COLOR_RED);
    tramp_curve = Semi(tramp_x,tramp_y + tramp_h/2.0f,tramp_h - 0.1f,180,COLOR_YELLOW);
    flore = Rectangle(0,-3.5,100,3,0,0,COLOR_BROWN);

    spikes[0] = Triangle(spikes_x,spikes_y,COLOR_YELLOW);
    spikes[1] = Triangle(spikes_x - spikes_dist,spikes_y,COLOR_YELLOW);
    spikes[2] = Triangle(spikes_x-2*spikes_dist,spikes_y,COLOR_YELLOW);

    for(int i=0;i<num;i++)
    {
        float vary = random(ball_var_start,ball_var_end);
        float vely = random(ball_vel_start,ball_vel_end);
        float rady = random(ball_rad_start,ball_rad_end);
        float xy = random(ball_x_start,ball_x_end);
        float sc;
        color_t cl;
        if(rand()%2)
        {
            cl = COLOR_RED;
            sc = 10;
        }
        else
        {
            cl = COLOR_YELLOW;
            sc = 20;
        }

        ball[i] = Ball(xy,vary,rady, -vely ,0,sc,cl);

        if(i < numslopes)
        {
            float ang = random(100,160);
            slope[i] = Rectangle(xy+rady*cos(ang*M_PI/180 - M_PI/2),vary+rady*sin(ang*M_PI/180 - M_PI/2),1.5,0.1,ang,-vely,COLOR_GREEN);
        }
    }


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
//    printf("%f %f %f %f %f %f \n",a.x,a.y,b.x,b.y,a.r,b.r);

    if( (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) < (a.r+b.r)*(a.r+b.r)){
        return true;
    }
    return false;
}

bool detect_collision_tramp(bounding_box_t a, bounding_box_r b)
{
    return ((a.y < b.y+b.l/2.0f)&&(a.x + a.r> b.x - b.w/2.0f)&&(a.x - a.r < b.x + b.w/2.0f));
}

bool detect_sides_tramp(bounding_box_t a, bounding_box_r b)
{
    return (a.x + a.r > b.x - tramp_w/2.0f && a.y < b.y + b.l);
}

bool detect_collision_floor(bounding_box_t a,bounding_box_r b,bounding_box_t pool)
{
//    return (a.y - a.r < b.y + 1.5f) && !((a.x < pool.x + pool.r) && (a.x > pool.x - pool.r));
    if(in_water)
         return ((a.x-pool.x)*(a.x-pool.x) + (a.y-pool.y)*(a.y-pool.y) > (a.r-pool.r)*(a.r-pool.r)) && (a.y <= pool.y);

//        cout << ((a.y-a.r-pool.y)*(a.y-a.r-pool.y) <= abs((a.x*a.x - pool.r*pool.r)));

    return (a.y - a.r < b.y + 1.5f) && !((a.x < pool.x + pool.r) && (a.x > pool.x - pool.r));

}

bool detect_pool_surface(bounding_box_t a,bounding_box_t pool)
{
    return ((a.x < pool.x + pool.r) && (a.x > pool.x - pool.r) && (a.y - a.r <= pool.y));
}

bool detect_spikes_surface(bounding_box_t a)
{
    return (spike_flag && (a.x < spikes_x + spikes_dist/2.0f)&&(a.x > spikes_x - 5*(spikes_dist/2.0f))&&(a.y < 0.4f - 2.0f + a.r ));
}

bool detect_slope_collision (bounding_box_t a,bounding_box_r b,float ang)
{
//    return (a.x-b.x <= a.r*sin(ang*M_PI/180) && a.x > b.x && a.y > b.y && a.y-b.y <= -a.r*cos(ang*M_PI/180));

    float theta = ang*M_PI/180;
    float m = tan(ang*theta);

//    return ((abs(m*(a.x - b.x) - (a.y - b.y))/sqrt(m*m + 1) <= a.r) && (a.x - a.r*cos(theta - 90) >= b.x + cos(theta)*b.w/2) && (a.x - a.r*cos(theta - 90) <= b.x - cos(theta)*b.w/2));
    return ((abs(m*(a.x - b.x) - (a.y - b.y))/sqrt(m*m + 1) <= a.r) && (a.y < b.y + 0.75) && (a.y > b.y - 0.75) && (a.x < b.x + 0.75) && (a.x > b.x - 0.75));
}

void reset_screen() {
    float top    = screen_center_y + 5 / screen_zoom;
    float bottom = screen_center_y - 5 / screen_zoom;
    float left   = screen_center_x - 5 / screen_zoom;
    float right  = screen_center_x + 5 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
