#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowheight) //our check function 
{
    return data.pos.y >= windowheight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame) 
{
    // update running time and frame
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        // update animation frame
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }

    }
    return data;
}

int main()
{
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    // initialize window
    InitWindow(windowDimensions[0], windowDimensions[1], "Bugra's New Project Window");



    const int gravity{1'000}; // gravity constant

    //nebula variables
    Texture2D nebula = LoadTexture("textures/nebula.png");
    

    const int sizeOfNebulae{10};
  
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0 / 12.0;
        nebulae[i].pos.x = windowDimensions[0] + 300*i;

    }

    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x }; // finish line position


    // nebula x velocity (pixels/second)
    int nebVel{-200};


    //scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0 / 12.0;
    scarfyData.runningTime = 0.0;


    // is rectangle in air
    bool isInAir{};
    const int jumpVel{-600};
    int velocity{0}; // reset velocity

    // background visuals
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    bool collision{};
    
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second 
    while (!WindowShouldClose())
    {
        // delta time
        const float dT{GetFrameTime()}; 

        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dT; // for moving the back ground 
        if (bgX <= -background.width*2)
        {
            bgX = 0.0; // reset background position
        }

        mgX -= 40 * dT; //movement for midground
        if (mgX <= -midground.width*2)
        {
            mgX = 0.0; // reset comment for midground position
        }

        fgX -= 80 * dT; // movement for foreground
        if (fgX <= -foreground.width*2)
        {
            fgX = 0.0; // reset comment for foreground position
        }

        // draw the background, midground and foreground
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background,bg1Pos, 0.0, 2.0, WHITE); // draw background
        Vector2 bg2Pos{bgX + background.width*2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE); // for continuous background
        
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE); // draw midground
        Vector2 mg2Pos{mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE); // for continuous midground

        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE); // draw foreground
        Vector2 fg2Pos{fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE); // for continuous foreground

       // perform ground check
        if (isOnGround(scarfyData, windowDimensions[1])) // updated with function
        {
            velocity = 0; // reset velocity
            isInAir = false; // update isInAir status
        }
        
        else
        {
            velocity += gravity * dT; // update position
            isInAir = true; // update isInAir status
        }


        // perform jump & jump check
        if (IsKeyPressed(KEY_UP) && !isInAir)
        {
            velocity += jumpVel; // jump velocity
        }
        
    
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            //update nebula position
            nebulae[i].pos.x += nebVel * dT;
        }

        finishLine += nebVel * dT; // setting finish line 
    
        //update scarfy position 
        scarfyData.pos.y += velocity * dT; // update position

        // update scarfy animation
        if(!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }
  
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

       
        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }

        }

        if (collision)
        {
            DrawText("Game Over!", windowDimensions[0]/4, windowDimensions[1]/2, 40, RED);

        }

        else if (scarfyData.pos.x >= finishLine)
        {
            DrawText("YOU WIN!", windowDimensions[0]/4, windowDimensions[1]/2, 40, GREEN);
        }

        else 

        {
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE); // draw nebula
            }
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE); // draw Scarfy
        }
        

        
    
        

        //stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy); // Unload texture
    UnloadTexture(nebula); // Unload texture
    UnloadTexture(background); // Unload texture
    UnloadTexture(midground); // Unload texture
    UnloadTexture(foreground); // Unload texture
    CloseWindow();

}