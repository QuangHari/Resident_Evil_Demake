#include "Player.h"

#define GRAVITY 1
#define MAX_FALL_SPEED 8
#define PLAYER_SPEED 35
#define PLAYER_JUMP_VAL 20
#define HEIGHT_BULLET 0.45
#define HETGHT_BULLET_RUN 0.5

Player::Player()
{
    frame = 0;
    x_pos = 0;
    y_pos = 0;
    x_val = 0;
    y_val = 0;
    width_frame = 0;
    height_frame = 0;
    status = WALK_NONE;
    hp =3;
    muteMusic = false;
    muteEffect = false;

    input_type.down=0;
    input_type.left=0;
    input_type.right=0;
    input_type.up=0;
    input_type.jump=0;
    on_ground =false;
    map_x = 0;
    map_y = 0;
    number_bullet = 10;
    number_bullet_left = 40;
    meetBoss = false;

}

Player::~Player()
{

}
bool Player::loadImg(std::string path, SDL_Renderer* screen){
    bool ret =Object::loadImg(path,screen);
    if (ret == TRUE){
        width_frame = rect.w/8;
        height_frame = rect.h;
    }
    return ret;
}

void Player::setClip(){
    if (width_frame >0 && height_frame >0){
        for (int i =0;i<8;i++){
            frame_clip[i].x = i*width_frame;
            frame_clip[i].y = 0;
            frame_clip[i].w = width_frame;
            frame_clip[i].h = height_frame;
        }

    }
}
void Player::Show(SDL_Renderer* des){
    updateImage(des);
    if (input_type.left ==1 || input_type.right ==1 ){
        frame ++;
        if (frame >7){
            frame -= 8;
        }
    }else {
        frame = 0;
    }

    if (come_back_time ==0){
        rect.x = x_pos - map_x;
        rect.y = y_pos -map_y;

        SDL_Rect* current_clip = &frame_clip[frame];
        SDL_Rect renderQuad = {rect.x,rect.y,width_frame,height_frame};
        SDL_RenderCopy(des, p_object,current_clip, &renderQuad);
    }

}

void Player::handleInputAction(SDL_Event event,SDL_Renderer* screen,Mix_Chunk* chunk,Mix_Chunk* chunk2,Mix_Chunk* chunk3,Mix_Chunk* chunk4){
    fall = chunk4;
    if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
            case SDLK_w:
                {
                    input_type.up =1;
                    input_type.down =0;
                }
                break;
            case SDLK_s:
                {
                    input_type.down =1;
                    input_type.up =0;
                }
                break;

            case SDLK_d:
                {
                    status = WALK_RIGHT;
                    input_type.right =1 ;
                    input_type.left =0 ;
                    updateImage(screen);
                }
                break;
            case SDLK_a:
                {
                    status = WALK_LEFT;
                    input_type.left =1 ;
                    input_type.right =0 ;
                    updateImage(screen);
                }
                break;
            case SDLK_k:
                {
                    if (on_ground ==true ){
                        input_type.jump =1;
                    }
                }
                break;
            case SDLK_j:
                {
                    if(number_bullet>0){

                        number_bullet --;
                        Bullet* p_bullet = new Bullet();

                        if(status == WALK_LEFT){
                            if (input_type.down == 0 && input_type.up == 0){
                                p_bullet->loadImg("img//_bullet.png",screen);
                                p_bullet->setDir(Bullet::DIR_LEFT);
                                p_bullet->setRect(this->rect.x -3,rect .y +height_frame*HEIGHT_BULLET);
                            }else if (input_type.down == 1){
                                p_bullet->loadImg("img//_bullet7.png",screen);
                                p_bullet->setDir(Bullet::DIR_DOWN_LEFT);
                                p_bullet->setRect(this->rect.x -3,rect .y +height_frame*HEIGHT_BULLET);
                            }else if (input_type.up == 1){
                                p_bullet->loadImg("img//_bullet6.png",screen);
                                p_bullet->setDir(Bullet::DIR_UP_LEFT);
                                p_bullet->setRect(this->rect.x -3,rect .y +height_frame*HEIGHT_BULLET);
                            }
                        }else if (status = WALK_RIGHT){
                            if (input_type.down == 0 && input_type.up == 0){
                                p_bullet->loadImg("img//_bullet2.png",screen);
                                p_bullet->setDir(Bullet::DIR_RIGHT);
                                p_bullet->setRect(this->rect.x +width_frame-20,rect .y +height_frame*HEIGHT_BULLET);
                            }else if (input_type.down == 1){
                                p_bullet->loadImg("img//_bullet5.png",screen);
                                p_bullet->setDir(Bullet::DIR_DOWN_RIGHT);
                                p_bullet->setRect(this->rect.x +width_frame-20,rect .y +height_frame*HEIGHT_BULLET);
                            }else if (input_type.up == 1){
                                p_bullet->loadImg("img//_bullet4.png",screen);
                                p_bullet->setDir(Bullet::DIR_UP_RIGHT);
                                p_bullet->setRect(this->rect.x +width_frame-20,rect .y +height_frame*HEIGHT_BULLET);
                            }

                        }

                        p_bullet->setXVal(20);
                        p_bullet->setYVal(20);
                        p_bullet->setMove(true);
                        p_bullet_list.push_back(p_bullet);
                        if (!muteEffect){
                            Mix_PlayChannel(-1,chunk,0);
                        }
                    }else{
                        if (!muteEffect){
                            Mix_PlayChannel(-1,chunk2,0);
                        }
                    }

                }
                break;
    //
            case SDLK_m:
                {
                    if (!muteMusic){
                        muteMusic = true;
                    }else {
                        muteMusic = false;
                    }
                }
                break;
            case SDLK_n:
                {
                    if (!muteEffect){
                        muteEffect = true;
                    }else {
                        muteEffect = false;
                    }
                }
                break;
            case SDLK_l:
                {
                    reload(chunk3);
                }
                break;

            default :
                break;
        }
    }else if (event.type == SDL_KEYUP){
        switch (event.key.keysym.sym){
            case SDLK_d:
                {
                    input_type.right =0 ;
                }
                break;
            case SDLK_a:
                {
                    input_type.left =0 ;
                }
                break;
            case SDLK_w:
                {
                    input_type.up =0 ;
                }
                break;
            case SDLK_s:
                {
                    input_type.down =0 ;
                }
                break;
            case SDLK_j:
                {

                }
                break;

        }
    }
}
void Player::reload(Mix_Chunk* sound){
    if (number_bullet <10 && number_bullet_left >0){
        if (10-number_bullet>=number_bullet_left){
            number_bullet+=number_bullet_left;
            number_bullet_left = 0;
        }else {
            number_bullet_left = number_bullet_left - (10-number_bullet);
            number_bullet = 10;

        }
        if (!muteEffect){
            Mix_PlayChannel(-1,sound,0);
        }

    }
}

void Player::Doplayer(Map& map_data){
    if (come_back_time ==0){
        x_val = 0 ;
        y_val += 0.8;

        if (y_val >= MAX_FALL_SPEED){
            y_val = MAX_FALL_SPEED;
        }
        if (input_type.left == 1){
            x_val -= PLAYER_SPEED;
        }
        if (input_type.right == 1){
            x_val += PLAYER_SPEED;
        }
        checkMap(map_data);
        if (input_type.jump ==1 ){
            if (on_ground == true){
                if (y_val >0){

                }else {
                    y_val -=PLAYER_JUMP_VAL;
                    input_type.jump =0;
                    on_ground =false;
                }

            }
        }
        centerPlayerOnMap(map_data);


    }else if (come_back_time >0){
        come_back_time --;
        if (come_back_time ==0){
            on_ground =false;
            y_pos =0;
            x_val =0;
            y_val=0;
            if (meetBoss == false){
                if (x_pos >256){
                    x_pos -=256;
                }else {
                    x_pos =0;
                }
            }else {
                x_pos = map_data.start_x +SCREEN_WIDTH/2;
            }

        }
    }
}

void Player::checkMap(Map& map_data){
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    int height_min = height_frame <TILE_SIZE ? height_frame : TILE_SIZE;
    x1 = (x_pos + x_val)/TILE_SIZE;
    x2 = (x_pos + x_val + width_frame -1)/TILE_SIZE;

    y1 = (y_pos )/TILE_SIZE;
    y1 = (y_pos  + height_min -1 )/TILE_SIZE;

    if (x1 >=0 && x2 <MAX_MAP_X && y1>=0 && y2 <MAX_MAP_Y){
        if (x_val>0){
            if (map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2]!= BLANK_TILE){
                x_pos = x2*TILE_SIZE;
                x_pos -= width_frame +1;
                x_val =0;

            }
        }else if (x_val < 0){
            if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1]!= BLANK_TILE){
                x_pos = (x1+1)*TILE_SIZE;
                x_val =0;
            }
        }
    }



    int width_min = width_frame <TILE_SIZE ? width_frame : TILE_SIZE;
    x1 =(x_pos)/TILE_SIZE;
    x2 = (x_pos +width_min)/TILE_SIZE;

    y1 = (y_pos +y_val)/TILE_SIZE;
    y2 = (y_pos +y_val + height_frame -1)/TILE_SIZE;
    if (x1 >=0 && x2 <MAX_MAP_X && y1 >= 0 && y2<MAX_MAP_Y){
        if (y_val >0){
            if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2]!= BLANK_TILE){
                y_pos = (y2)*TILE_SIZE;
                y_pos -= (height_frame+1);
                y_val = 0;
                if (status ==WALK_NONE){
                    status =WALK_RIGHT;
                }
                on_ground =true;
            }
        }else if (y_val < 0 ){
            if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2]!= BLANK_TILE){
                y_pos = (y1+1)*TILE_SIZE;
                y_val = 0;
            }
        }
    }

    if (meetBoss == true){
        if (x_pos<map_data.start_x+100 ){
            if (x_val <0){
                x_val = 0;
            }
        }
    }
    x_pos +=x_val;
    y_pos += y_val;

    if (x_pos <0){
        if (meetBoss == true){

                x_pos = 200;


        }else {
            x_pos =0;
        }
    }else if (x_pos +width_frame >map_data.max_x){
        x_pos = map_data.max_x -width_frame -1;
    }
    if (y_pos >map_data.max_y){
        come_back_time = 60;
        Mix_PlayChannel(-1,fall,0);
        hp -- ;


    }
}

void Player::centerPlayerOnMap(Map& map_data){
    if (meetBoss == false){
        map_data.start_x = x_pos -(SCREEN_WIDTH/2);
        if (map_data.start_x <0){
            map_data.start_x = 0;
        }else if (map_data.start_x +SCREEN_WIDTH >=map_data.max_x){
            map_data.start_x = map_data.max_x - SCREEN_WIDTH;
            meetBoss = true;

        }
    }


    map_data.start_y = y_pos - (SCREEN_HEIGHT/2);
    if (map_data.start_y <0){
        map_data.start_y = 0;
    }else if (map_data.start_y +SCREEN_HEIGHT >=map_data.max_y){
        map_data.start_y = map_data.max_y - SCREEN_HEIGHT;

    }

}
void Player::updateImage(SDL_Renderer* des){
    if (on_ground == true){
        if (status == WALK_LEFT){
            loadImg("img//soldier_left.png",des);
        }else {
            loadImg("img//soldier_right.png",des);
        }

    }else {
        if (status == WALK_LEFT){
            loadImg("img//soldier_left.png",des);
        }else {
            loadImg("img//soldier_right.png",des);
        }
    }
}

void Player::handleBullet(SDL_Renderer* des){
    for (int i=0;i<p_bullet_list.size();i++){
        Bullet* bullet =p_bullet_list.at(i);
        if (bullet != NULL){
            if(bullet->getMove() ==true){
                bullet->handleMove(SCREEN_WIDTH,SCREEN_HEIGHT);
                bullet->render(des);
            }else {
                p_bullet_list.erase(p_bullet_list.begin() +i);
                if (bullet !=NULL){
                    delete bullet;
                    bullet =NULL;
                }
            }
        }
    }
}

SDL_Rect Player::getRectFrame(){
    SDL_Rect rectPlayer;
    rectPlayer.x = rect.x +30;
    rectPlayer.y = rect.y;
    rectPlayer.w = width_frame -30;
    rectPlayer.h = height_frame -1;
    return rectPlayer;
}

void Player::removeBullet(const int& idx){
    int size =p_bullet_list.size();
    if (size >0 && idx <size){
        Bullet* p_bullet =p_bullet_list.at(idx);
        p_bullet_list.erase(p_bullet_list.begin() + idx);
        if (p_bullet){
            delete p_bullet;
            p_bullet = NULL;

        }
    }
}


