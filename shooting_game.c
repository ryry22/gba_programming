#define VRAM 0x06000000
#define cyan 0x7FE0
#define gray 0x6318
#define red 0x001F
#define yellow 0x03FF
#define blue 0x7C00
#define white 0x7FFF
#define black 0x0000
#define green 0x03E0
#define mazenta 0x7C1F
#define yellowgreen 0x03F8
#define purple 0x2409


#include "gba1.h"
void draw_plane(hword, hword);
void draw_line(hword, hword, hword, hword, hword);
int search_gray(hword,hword);
void erasedraw_plane(hword, hword);
void lasergun(hword, hword, hword);
void display_time(hword);
void draw_enemy(hword,hword);
void erasedraw_enemy(hword,hword,hword);
int search_purple(hword,hword);
void enemyattack1(hword,hword,hword);
void enemyattack2Left(hword,hword,hword);
void enemyattack2Right(hword,hword,hword);
void draw_boss(hword x1, hword y1);
void erasedraw_boss(hword x1, hword y1);

unsigned char plane[21][21]= {
  {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,2,2,2,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,2,2,2,1,0,0,0,0,0,0,0,0},
  {1,0,0,0,1,0,0,0,1,2,2,2,1,0,0,0,1,0,0,0,1},
  {1,0,0,0,1,0,0,1,1,1,1,1,1,1,0,0,1,0,0,0,1},
  {1,0,0,0,1,0,1,1,1,1,5,1,1,1,1,0,1,0,0,0,1},
  {1,0,0,0,1,1,5,1,1,1,5,1,1,1,5,1,1,0,0,0,1},
  {1,0,0,1,1,1,5,1,1,1,5,1,1,1,5,1,1,1,0,0,1},
  {1,0,1,1,1,1,5,1,1,1,1,1,1,1,5,1,1,1,1,0,1},
  {1,1,1,1,1,1,5,1,1,1,1,1,1,1,5,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,5,1,1,1,1,1,5,1,1,1,1,1,1,1},
  {0,0,0,0,0,1,1,5,1,1,0,1,1,5,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,6,6,6,1,0,1,6,6,6,1,1,0,0,0,0},
  {0,0,0,1,1,1,6,6,6,1,0,1,6,6,6,1,1,1,0,0,0},
  {0,0,0,0,0,0,3,4,3,0,0,0,3,4,3,0,0,0,0,0,0},
  {0,0,0,0,0,0,3,4,3,0,0,0,3,4,3,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,3,0,0,0,0,0,3,0,0,0,0,0,0,0}
};

unsigned char enemy[12][13]= {
  {0,0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,0,0},
  {0,0,1,2,2,1,1,1,2,2,1,0,0},
  {0,1,1,1,2,2,1,2,2,1,1,1,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,0,1,2,2,2,2,2,2,2,1,0,0},
  {0,1,1,1,2,2,2,2,2,1,1,1,0},
  {1,1,1,1,1,2,2,2,1,1,1,1,1},
  {0,1,1,0,1,1,1,1,1,0,1,1,0},
  {0,1,1,1,0,0,0,0,0,1,1,1,0},
  {1,0,1,0,1,0,0,0,1,0,1,0,1}
};
hword *input; //キー判定



//gba 240*160
int main(void) {
	/* 画面初期化ルーチン */
	*((unsigned short *)IOBASE) = 0xF03;
	input = 0x04000130;
	draw_enemy(0,0);
	draw_enemy(227,0);
	draw_enemy(0,148);
	draw_enemy(227,148);
	locate(10,8);
	prints("SPACE SHOOT");
	locate(4,14);
	prints("START:PRESS START-KEY");
	locate(4,17);
	prints("move:LEFT OR RIGHT KEY");
	locate(8,18);
	prints("attack:UP-KEY");
  locate(10,19);
	prints("cheat:A-KEY");

    while(1) {
        if((*input & KEY_START) != KEY_START) {
            break;
        }
    }

  hword *ptr,sflag,MyHP,myalive,gameclear;
  hword shotc,mlx1,mly1,lgflag1,mlx2,mly2,lgflag2,mlx3,mly3,lgflag3,mlx4,mly4,lgflag4,mlx5,mly5,lgflag5,mlx6,mly6,lgflag6,mlx7,mly7,lgflag7,mlx8,mly8,lgflag8;
	myalive = 1;
	shotc = 0;
  sflag = 0;
	MyHP = 20;
  int flag = 0;
  volatile unsigned short time;
	int score = 0;
  int i,j,res,bossstage;
  res = 1;

	/* 画面初期化ルーチン */
	*((unsigned short *)IOBASE) = 0xF03;
	for(i=0;i<160;i++) {
		for(j=0;j<240;j++) {
			draw_point(j,i,black);
		}
	}
	/* タイマカウンタ設定レジスタ */
	*((unsigned short *)0x04000100) = 0xFF00;	// タイマ0 カウンタ設定(元クロック1/2^24秒 約60n秒)
	*((unsigned short *)0x04000104) = 0xFF00;	// タイマ1 カウンタ設定(元クロック1/2^16秒 約15.4u秒)
	*((unsigned short *)0x04000108) = 0xFFE5;	// タイマ2 カウンタ設定(元クロック10/2^8秒  約39.3m秒)
	*((unsigned short *)0x0400010C) = 0x0000;	// タイマ3 カウンタ設定(元クロック0.1秒)

	/* タイマ制御レジスタ */
	*((unsigned short *)0x04000102) = 0x0080;	// タイマ0 制御設定（タイマON，割込みOFF，カスケードOFF，プリスケーラなし）
	*((unsigned short *)0x04000106) = 0x0084;	// タイマ1 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010A) = 0x0084;	// タイマ2 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010E) = 0x0084;	// タイマ3 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）


  int t;
	ptr=(hword*) 0x04000000;
	*ptr=0x0F03;


    hword px,py;
    px = 110;
    py = 139;
    draw_plane(px,py);

	//敵1の変数
  hword enemyHP1,enx1,eny1,eaflag,menx1,meny1,timing,timing2,timing3,e1alive,eaflag2,menx2,meny2,eaflag3,menx3,meny3,Switch1,Switch2,e,move;
  int movetiming,LRflag;
  LRflag = 0;
  //敵2の変数
  hword enemyHP2second,enx2second,eny2second,eaflagsecond,menx2second,meny2second,timing12second,timing22second,timing32second,seconde2alivesecond,secondeaflag2second,secondmenx2second,secondmeny2second,secondeaflag3second,secondmenx3second,secondmeny3second,secondSwitch1second,secondSwitch2second,e2second,move2second;
  int movetiming2second,LRflag2second;
  LRflag2second = 0;
  //ボスの変数
  hword HP1boss,x1boss,y1boss,aflagboss,mx1boss,my1boss,timingboss,timing2boss,timing3boss,aliveboss,aflag2boss,mx2boss,my2boss,aflag3boss,mx3boss,my3boss,Switch1boss,Switch2boss,eboss,moveboss;
  int movetimingboss,LRflagboss,changemoveboss,moveflag;
  LRflagboss = 0;
  changemoveboss = 0;
  moveflag=0;

  //敵1の設定
  enemyHP1 = 15;
	move = 0;
	e = 0;
	Switch1 = 0;
	Switch2 = 0;
	eaflag = 0;
	enx1 = 140;
	eny1 = 30;
  //敵2の設定
  enemyHP2second = 15;
	move2second = 0;
	e2second = 0;
	secondSwitch1second = 0;
	secondSwitch2second = 0;
	secondeaflag2second = 0;
	enx2second = 90;
	eny2second = 30;
  //ボスの設定
  HP1boss = 20;
	moveboss = 0;
	eboss = 0;
	Switch1boss = 0;
	Switch2boss = 0;
	aflagboss = 0;
	x1boss = 110;
	y1boss = 25;

	//初期；敵の生成
  draw_enemy(enx1,eny1);
	e1alive = 1;
  draw_enemy(enx2second,eny2second);
	seconde2alivesecond = 1;

	//開始まで時間制御
	for(i=0;i<300000;i++) {}


//繰り返し行う処理
  while(1) {
		time = 	*((unsigned short *)0x0400010C);
    locate(21,0);
  	prints("Time:");
		locate(26, 0);
		display_time(time);

    if((*input & KEY_A) != KEY_A) {
      enemyHP1 = 0;
      enemyHP2second = 0;
    }

    //キー判定・左右移動
    if((*input & KEY_LEFT) != KEY_LEFT && px>0 && myalive == 1){
      erasedraw_plane(px,py);
			px--;
      draw_plane(px,py);
      flag = 1;
		} else if((*input & KEY_RIGHT) != KEY_RIGHT && px<219 && myalive == 1) {
      erasedraw_plane(px,py);
			px++;
      draw_plane(px,py);
      flag = 1;
    } else {
			for(t = 0;t < 8000;t++) {}
		}

   //キー判定・上矢印が押されたときにそのときの座上をコピーし、弾丸を発射する
    if((*input & KEY_UP) != KEY_UP && myalive == 1) {
      if(shotc == 0 && lgflag1 == 0 && sflag == 0) {
        mlx1 = px;
        mly1 = py;
        lgflag1 = 1;
        shotc++;
      }
      if(shotc == 1 && lgflag2 == 0 && sflag == 0) {
        mlx2 = px;
        mly2 = py;
        lgflag2 = 1;
        shotc++;
      }
      else if(shotc == 2 && lgflag3 == 0 && sflag == 0) {
        mlx3 = px;
        mly3 = py;
        lgflag3 = 1;
        shotc++;
      }
      else if(shotc == 3 && lgflag4 == 0 && sflag == 0) {
        mlx4 = px;
        mly4 = py;
        lgflag4 = 1;
        shotc++;
      }
      else if(shotc == 4 && lgflag5 == 0 && sflag == 0) {
        mlx5 = px;
        mly5 = py;
        lgflag5 = 1;
        shotc++;
      }
      else if(shotc == 5 && lgflag6 == 0 && sflag == 0) {
        mlx6 = px;
        mly6 = py;
        lgflag6 = 1;
        shotc++;
      }
      else if(shotc == 6 && lgflag7 == 0 && sflag == 0) {
        mlx7 = px;
        mly7 = py;
        lgflag7 = 1;
        shotc++;
      }
      else if(shotc == 7 && lgflag8 == 0 && sflag == 0) {
        mlx8 = px;
        mly8 = py;
        lgflag8 = 1;
        shotc++;
      }
      else if(shotc == 8) {
        shotc = 0;
      }
      sflag = 1;
    } else {
      sflag = 0;
    }




    //弾丸の移動と進む先のメモリが紫の値を保持（search_purple）していれば当たり判定
    if(lgflag1 == 1) {
      if(search_purple(mlx1,mly1-2) == 1 || search_purple(mlx1+4,mly1-2) == 1 || search_purple(mlx1+10,mly1-2) == 1 || search_purple(mlx1+16,mly1-2) == 1 || search_purple(mlx1+20,mly1-2) == 1) {
        lasergun(mlx1,mly1,black);
        lgflag1 = 0;
        if(bossstage == 1) {
			HP1boss--;
		} else {
          enemyHP1--;
          enemyHP2second--;
        }
      } else {
        lasergun(mlx1,mly1,black);
        mly1--;
        lasergun(mlx1,mly1,yellowgreen);
      }
      if(mly1 == 0) {
        lgflag1 = 0;
    }
  }
    if(lgflag2 == 1 && mly2 > 0) {
      if(search_purple(mlx2,mly2-2) == 1 || search_purple(mlx2+4,mly2-2) == 1 || search_purple(mlx2+10,mly2-2) == 1 || search_purple(mlx2+16,mly2-2) == 1 || search_purple(mlx2+20,mly2-2) == 1)  {
        lasergun(mlx2,mly2,black);
        lgflag2 = 0;
        if(bossstage == 1) {
					  HP1boss--;
				} else {
          enemyHP1--;
          enemyHP2second--;
        }
				score = score + 10;
      } else {
        lasergun(mlx2,mly2,black);
        mly2--;
        lasergun(mlx2,mly2,yellowgreen);
      }
     if(mly2 == 0) {
        lgflag2 = 0;
      }

    }

    if(lgflag3 == 1 && mly3 > 0) {
      if(search_purple(mlx3+10,mly3-2) == 1 || search_purple(mlx3,mly3-2) == 1 || search_purple(mlx3+4,mly3-2) == 1 || search_purple(mlx3+16,mly3-2) == 1 || search_purple(mlx3+20,mly3-2) == 1)  {
        lasergun(mlx3,mly3,black);
        lgflag3 = 0;
        if(bossstage == 1) {
					  HP1boss--;
				} else {
          enemyHP1--;
          enemyHP2second--;
        }
				score = score + 10;
      }else {
        lasergun(mlx3,mly3,black);
        mly3--;
        lasergun(mlx3,mly3,yellowgreen);
      }
      if(mly3 == 0) {
        lgflag3 = 0;
      }
    }

    if(lgflag4 == 1 && mly4 > 0) {
      if(search_purple(mlx4+10,mly4-2) == 1 || search_purple(mlx4,mly4-2) == 1 || search_purple(mlx4+4,mly4-2) == 1 || search_purple(mlx4+16,mly4-2) == 1 || search_purple(mlx4+20,mly4-2) == 1)  {
        lasergun(mlx4,mly4,black);
        lgflag4 = 0;
        if(bossstage == 1) {
					  HP1boss--;
				} else {
          enemyHP1--;
          enemyHP2second--;
        }
				score = score + 10;
      }  else {
        lasergun(mlx4,mly4,black);
        mly4--;
        lasergun(mlx4,mly4,yellowgreen);
      }
      if(mly4 == 0) {
        lgflag4 = 0;
      }
    }

    if(lgflag5 == 1 && mly5 > 0) {
      if(search_purple(mlx5+10,mly5-2) == 1 || search_purple(mlx5,mly5-2) == 1 || search_purple(mlx5+4,mly5-2) == 1 || search_purple(mlx5+16,mly5-2) == 1 || search_purple(mlx5+20,mly5-2) == 1)  {
				lasergun(mlx5,mly5,black);
				lgflag5 = 0;
        if(bossstage == 1) {
					  HP1boss--;
				} else {
          enemyHP1--;
          enemyHP2second--;
        }
				score = score + 10;
      }  else {
        lasergun(mlx5,mly5,black);
        mly5--;
        lasergun(mlx5,mly5,yellowgreen);
      }
      if(mly5 == 0) {
        lgflag5 = 0;
      }
    }

    if(lgflag6 == 1 && mly6 > 0) {
      if(search_purple(mlx6+10,mly6-2) == 1 || search_purple(mlx6,mly6-2) == 1 || search_purple(mlx6+4,mly6-2) == 1 || search_purple(mlx6+16,mly6-2) == 1 || search_purple(mlx6+20,mly6-2) == 1)  {
        lasergun(mlx6,mly6,black);
        lgflag6 = 0;
        if(bossstage == 1) {
					  HP1boss--;
				} else {
          enemyHP1--;
          enemyHP2second--;
        }
				score = score + 10;
      }
      else {
        lasergun(mlx6,mly6,black);
        mly6--;
        lasergun(mlx6,mly6,yellowgreen);
      }
      if(mly6 == 0) {
        lgflag6 = 0;
      }
    }

    if(lgflag7 == 1 && mly7 > 0) {
      if(search_purple(mlx7+10,mly7-2) == 1 || search_purple(mlx7,mly7-2) == 1 || search_purple(mlx7+4,mly7-2) == 1 || search_purple(mlx7+16,mly7-2) == 1 || search_purple(mlx7+20,mly7-2) == 1)  {
        lasergun(mlx7,mly7,black);
        lgflag7 = 0;
        if(bossstage == 1) {
					  HP1boss--;
				} else {
          enemyHP1--;
          enemyHP2second--;
        }
				score = score + 10;
      } else {
        lasergun(mlx7,mly7,black);
        mly7--;
        lasergun(mlx7,mly7,yellowgreen);
      }
      if(mly7 == 0) {
        lgflag7 = 0;
      }
    }

    if(lgflag8 == 1 && mly8 > 0) {
      if(search_purple(mlx8+10,mly8-2) == 1 || search_purple(mlx8,mly8-2) == 1 || search_purple(mlx8+4,mly8-2) == 1 || search_purple(mlx8+16,mly8-2) == 1 || search_purple(mlx8+20,mly8-2) == 1)  {
        lasergun(mlx8,mly8,black);
        lgflag8 = 0;
        if(bossstage == 1) {
					  HP1boss--;
				} else {
          enemyHP1--;
          enemyHP2second--;
        }
				score = score + 10;
      } else {
        lasergun(mlx8,mly8,black);
        mly8--;
        lasergun(mlx8,mly8,yellowgreen);
      }
      if(mly8 == 0) {
        lgflag8 = 0;
      }
    }

		//敵1のmove
		//2秒毎に敵がストレート攻撃を開始する
		timing = mod(time, 20);
		if(timing == 0 && eaflag == 0 && e1alive == 1 && myalive != 0) {
			menx1 = enx1;
			meny1 = eny1;
			eaflag = 1;
		}
		//敵のストレート攻撃の移動と自分への当たり判定
		if(eaflag == 1) {
			if(search_gray(menx1,meny1+12) == 1 || search_gray(menx1+2,meny1+12) == 1 || search_gray(menx1+4,meny1+12) == 1 || search_gray(menx1+8,meny1+12) == 1 || search_gray(menx1+10,meny1+12) == 1 || search_gray(menx1+12,meny1+12) == 1) {
				enemyattack1(menx1,meny1,black);
				eaflag == 0;
				MyHP--;
			} else {
				enemyattack1(menx1,meny1,black);
				meny1++;
				enemyattack1(menx1,meny1,red);
			}
			if(meny1==160) {
				eaflag = 0;
			}
		}




		//5秒毎に敵が波状攻撃を開始する
		timing2 = mod(time, 30);
		if(timing2 == 0 && eaflag2 == 0 && e1alive == 1 && myalive != 0) {
			menx2 = enx1;
			meny2 = eny1;
			menx3 = enx1;
			meny3 = eny1;
			eaflag2 = 1;
			eaflag3 = 1;
		}
		//敵の波状攻撃
		if(eaflag2 == 1) {
			//左波状攻撃
			if(search_gray(menx2-4,meny2+12) == 1 || search_gray(menx2-2,meny2+12) == 1 || search_gray(menx2,meny2+12) == 1 || search_gray(menx2+2,meny2+12) == 1 || search_gray(menx2+4,meny2+12) == 1 || search_gray(menx2+6,meny2+12) == 1) {
				enemyattack2Left(menx2,meny2,black);
				eaflag2 == 0;
				MyHP--;
			} else {
				enemyattack2Left(menx2,meny2,black);
				if(Switch1 == 0)  {
					meny2 = meny2 + 2;
					Switch1 = 1;
				} else {
					menx2--;
					Switch1 = 0;
				}
				enemyattack2Left(menx2,meny2,red);
			}
			if(meny2==160 || menx2-4 == 0) {
				enemyattack2Left(menx2,meny2,black);
				eaflag2 = 0;
			}
		}

		if(eaflag3 == 1) {
			//右波状攻撃
			if(search_gray(menx3+8,meny3+12) == 1 || search_gray(menx3+10,meny3+12) == 1 || search_gray(menx3+12,meny3+12) == 1 || search_gray(menx3+14,meny3+12) == 1 || search_gray(menx3+16,meny3+12) == 1 || search_gray(menx3+18,meny3+12) == 1) {
				enemyattack2Right(menx3,meny3,black);
				eaflag3 == 0;
				MyHP--;
			} else {
				enemyattack2Right(menx3,meny3,black);
				if(Switch2 == 0)  {
					meny3 = meny3+2;
					Switch2 = 1;
				} else {
					menx3++;
					Switch2 = 0;
				}
				enemyattack2Right(menx3,meny3,red);
			}
			if(meny3 == 160 || menx3+18 == 240) {
				enemyattack2Right(menx3,meny3,black);
				eaflag3 = 0;
			}
		}



		//敵1の動き
		if(e1alive == 1  && LRflag == 0 && myalive == 1) {
			erasedraw_enemy(enx1,eny1,black);
				enx1++;
				move++;
			draw_enemy(enx1,eny1);
		}else if(e1alive == 1  && LRflag == 1 && myalive == 1) {
			erasedraw_enemy(enx1,eny1,black);
				enx1--;
				move++;
			draw_enemy(enx1,eny1);
		}

        if(LRflag == 0 && move == 90) {
            LRflag = 1;
            move = 0;
        } else if(LRflag == 1 && move == 90) {
            LRflag = 0;
            move = 0;
        }



//敵2の攻撃
        //2秒毎に敵がストレート攻撃を開始する
        if(timing == 0 && eaflagsecond == 0 && seconde2alivesecond == 1 && myalive != 0) {
          menx2second = enx2second;
          meny2second = eny2second;
          eaflagsecond = 1;
        }
        //敵のストレート攻撃の移動と自分への当たり判定
        if(eaflagsecond == 1) {
          if(search_gray(menx2second,meny2second+12) == 1 || search_gray(menx2second+2,meny2second+12) == 1 || search_gray(menx2second+4,meny2second+12) == 1 || search_gray(menx2second+8,meny2second+12) == 1 || search_gray(menx2second+10,meny2second+12) == 1 || search_gray(menx2second+12,meny2second+12) == 1) {
            enemyattack1(menx2second,meny2second,black);
            eaflag == 0;
            MyHP--;
          } else {
            enemyattack1(menx2second,meny2second,black);
            meny2second++;
            enemyattack1(menx2second,meny2second,red);
          }
          if(meny2second==160) {
            eaflagsecond = 0;
          }
        }



        //5秒毎に敵が波状攻撃を開始する
        timing2 = mod(time, 30);
        if(timing2 == 0 && secondeaflag2second == 0 && seconde2alivesecond == 1 && myalive != 0) {
          secondmenx2second = enx2second;
          secondmeny2second = eny2second;
          secondmenx3second = enx2second;
          secondmeny3second = eny2second;
          secondeaflag2second = 1;
          secondeaflag3second = 1;
        }
        //敵の波状攻撃
        if(secondeaflag2second == 1) {
          //左波状攻撃
          if(search_gray(secondmenx2second-4,secondmeny2second+12) == 1 || search_gray(secondmenx2second-2,secondmeny2second+12) == 1 || search_gray(secondmenx2second,secondmeny2second+12) == 1 || search_gray(secondmenx2second+2,secondmeny2second+12) == 1 || search_gray(secondmenx2second+4,secondmeny2second+12) == 1 || search_gray(secondmenx2second+6,secondmeny2second+12) == 1) {
            enemyattack2Left(secondmenx2second,secondmeny2second,black);
            secondeaflag2second == 0;
            MyHP--;
          } else {
            enemyattack2Left(secondmenx2second,secondmeny2second,black);
            if(secondSwitch1second == 0)  {
              secondmeny2second = secondmeny2second + 2;
              secondSwitch1second = 1;
            } else {
              secondmenx2second--;
              secondSwitch1second = 0;
            }
            enemyattack2Left(secondmenx2second,secondmeny2second,red);
          }
          if(secondmeny2second==160 || secondmenx2second-4 == 0) {
            enemyattack2Left(secondmenx2second,secondmeny2second,black);
            secondeaflag2second = 0;
          }
        }

        if(secondeaflag3second == 1) {
          //右波状攻撃
          if(search_gray(secondmenx3second+8,secondmeny3second+12) == 1 || search_gray(secondmenx3second+10,secondmeny3second+12) == 1 || search_gray(secondmenx3second+12,secondmeny3second+12) == 1 || search_gray(secondmenx3second+14,secondmeny3second+12) == 1 || search_gray(secondmenx3second+16,secondmeny3second+12) == 1 || search_gray(secondmenx3second+18,secondmeny3second+12) == 1) {
            enemyattack2Right(secondmenx3second,secondmeny3second,black);
            secondeaflag3second == 0;
            MyHP--;
          } else {
            enemyattack2Right(secondmenx3second,secondmeny3second,black);
            if(secondSwitch2second == 0)  {
              secondmeny3second = secondmeny3second+2;
              secondSwitch2second = 1;
            } else {
              secondmenx3second++;
              secondSwitch2second = 0;
            }
            enemyattack2Right(secondmenx3second,secondmeny3second,red);
          }
          if(secondmeny3second == 160 || secondmenx3second+18 == 240) {
            enemyattack2Right(secondmenx3second,secondmeny3second,black);
            secondeaflag3second = 0;
          }
        }




    //敵2の動き
    if(seconde2alivesecond == 1  && LRflag2second == 1 && myalive == 1) {
			erasedraw_enemy(enx2second,eny2second,black);
				enx2second++;
				move2second++;
			draw_enemy(enx2second,eny2second);
		}else if(seconde2alivesecond == 1  && LRflag2second == 0 && myalive == 1) {
			erasedraw_enemy(enx2second,eny2second,black);
				enx2second--;
				move2second++;
			draw_enemy(enx2second,eny2second);
		}

        if(LRflag2second == 0 && move2second == 90) {
            LRflag2second = 1;
            move2second = 0;
        } else if(LRflag2second == 1 && move2second == 90) {
            LRflag2second = 0;
            move2second = 0;
        }




//enemyHP1 or enemyHP2secondが0ならば敵を消す
    if(enemyHP1 == 0 || enemyHP2second == 0) {
			e1alive = 0;
      seconde2alivesecond = 0;
			if(e == 0 || e2second == 0) {
				erasedraw_enemy(enx1,eny1,black);
                erasedraw_enemy(enx2second,eny2second,black);
                aliveboss = 1;
                bossstage = 1;
                for(i=0;i<100;i++) {
                  locate(7,10);
                  prints("Defeat the boss!!");
                 }
                for(i=0;i<160;i++) {
                    for(j=0;j<240;j++) {
                        draw_point(j,i,black);
                    }
                }
                draw_boss(x1boss,y1boss);
								draw_plane(px,py);
			}
      e = 1;
      e2second = 1;

    }


    //敵を全部倒した後、ボスの登場
    if(bossstage == 1 && aliveboss == 1 && myalive == 1) {
      draw_boss(x1boss,y1boss);
      if(res = 1) {
        enemyHP1 = 10;
        enemyHP2second = 10;
        e1alive = 1;
        seconde2alivesecond = 1;
        res = 0;
      }
      //敵の動き、テレポートする
      timingboss = mod(time, 31);
      timing2boss = mod(time, 30);
      if(timing2boss==0 && moveflag==1) {
        moveflag = 0;
      }
      if(changemoveboss == 0 && timingboss==0 && moveflag == 0) {
        erasedraw_boss(x1boss,y1boss);
        x1boss = 30;
        draw_boss(x1boss,y1boss);
        changemoveboss = 1;
        moveflag = 1;
      } else if (changemoveboss == 1 && timingboss==0 && moveflag == 0) {
        erasedraw_boss(x1boss,y1boss);
        x1boss = 200;
        draw_boss(x1boss,y1boss);
        changemoveboss = 2;
        moveflag = 1;
      } else if(changemoveboss == 2 && timingboss==0 && moveflag == 0) {
        erasedraw_boss(x1boss,y1boss);
        x1boss = 110;
        draw_boss(x1boss,y1boss);
        changemoveboss = 0;
        moveflag = 1;
      }

      //ボスの攻撃
      //1.5秒毎に敵がストレート攻撃を開始する
      timing = mod(time, 15);
      if(timing == 0 && aflagboss == 0 && aliveboss == 1 && myalive != 0) {
        mx1boss = x1boss+5;
        my1boss = y1boss+8;
        aflagboss = 1;
      }
      //敵のストレート攻撃の移動と自分への当たり判定
      if(aflagboss == 1) {
        if(search_gray(mx1boss,my1boss+12) == 1 || search_gray(mx1boss+2,my1boss+12) == 1 || search_gray(mx1boss+4,my1boss+12) == 1 || search_gray(mx1boss+8,my1boss+12) == 1 || search_gray(mx1boss+10,my1boss+12) == 1 || search_gray(mx1boss+12,my1boss+12) == 1) {
          enemyattack1(mx1boss,my1boss,black);
          aflagboss == 0;
          MyHP--;
        } else {
          enemyattack1(mx1boss,my1boss,black);
          my1boss++;
          enemyattack1(mx1boss,my1boss,red);
        }
        if(my1boss==160) {
          aflagboss = 0;
        }
      }




      //2.5秒毎に敵が波状攻撃を開始する
      timing2 = mod(time, 25);
      if(timing2 == 0 && aflag2boss == 0 && aliveboss == 1 && myalive != 0) {
        mx2boss = x1boss+5;
        my2boss = y1boss+8;
        mx3boss = x1boss+5;
        my3boss = y1boss+8;
        aflag2boss = 1;
        aflag3boss = 1;
      }
      //敵の波状攻撃
      if(aflag2boss == 1) {
        //左波状攻撃
        if(search_gray(mx2boss-4,my2boss+12) == 1 || search_gray(mx2boss-2,my2boss+12) == 1 || search_gray(mx2boss,my2boss+12) == 1 || search_gray(mx2boss+2,my2boss+12) == 1 || search_gray(mx2boss+4,my2boss+12) == 1 || search_gray(mx2boss+6,my2boss+12) == 1) {
          enemyattack2Left(mx2boss,my2boss,black);
          aflag2boss == 0;
          MyHP--;
        } else {
          enemyattack2Left(mx2boss,my2boss,black);
          if(Switch1boss == 0)  {
            my2boss = my2boss + 2;
            Switch1boss = 1;
          } else {
            mx2boss--;
            Switch1boss = 0;
          }
          enemyattack2Left(mx2boss,my2boss,red);
        }
        if(my2boss==160 || mx2boss-4 == 0) {
          enemyattack2Left(mx2boss,my2boss,black);
          aflag2boss = 0;
        }
      }

      if(aflag3boss == 1) {
        //右波状攻撃
        if(search_gray(mx3boss+8,my3boss+12) == 1 || search_gray(mx3boss+10,my3boss+12) == 1 || search_gray(mx3boss+12,my3boss+12) == 1 || search_gray(mx3boss+14,my3boss+12) == 1 || search_gray(mx3boss+16,my3boss+12) == 1 || search_gray(mx3boss+18,my3boss+12) == 1) {
          enemyattack2Right(mx3boss,my3boss,black);
          aflag3boss == 0;
          MyHP--;
        } else {
          enemyattack2Right(mx3boss,my3boss,black);
          if(Switch2boss == 0)  {
            my3boss = my3boss+2;
            Switch2boss = 1;
          } else {
            mx3boss++;
            Switch2boss = 0;
          }
          enemyattack2Right(mx3boss,my3boss,red);
        }
        if(my3boss == 160 || mx3boss+18 == 240) {
          enemyattack2Right(mx3boss,my3boss,black);
          aflag3boss = 0;
        }
      }

    }

    //ゲームクリアの処理
        if(HP1boss <= 0 && myalive == 1) {
          *((hword *)0x04000102) = 0x0000;
    	  aliveboss = 0;
    	  erasedraw_boss(x1boss,y1boss);
          locate(7,9);
          prints("congratulation!!");
          locate(10,10);
          prints("Score:");
          locate(16,10);
          display_time(score);
          gameclear = 1;
        }

//HPが0になったらGG&&ゲームオーバーの処理
		if(MyHP == 0 && gameclear != 1 ) {
			myalive = 0;
            erasedraw_plane(px,py);
            erasedraw_boss(x1boss,y1boss);
            erasedraw_enemy(enx1,eny1,black);
            erasedraw_enemy(enx2second,eny2second,black);
            *((hword *)0x04000102) = 0x0000;
			locate(10,9);
			prints("GAME OVER");
			locate(10,10);
			prints("Score:");
			locate(16,10);
			display_time(score);
		}


  }
  return 0;
}



//メソッド
//戦闘機を描く
void draw_plane(hword x1, hword y1) {
  int i,j;
  for(i=0;i<21;i++) {
    for(j=0;j<21;j++) {
      if(plane[i][j]==1) {
        draw_point(x1+j,y1+i,gray);
      } else if(plane[i][j]==2) {
        draw_point(x1+j,y1+i,cyan);
      } else if(plane[i][j]==3) {
        draw_point(x1+j,y1+i,red);
      } else if(plane[i][j]==4) {
        draw_point(x1+j,y1+i,yellow);
      } else if(plane[i][j]==5) {
        draw_point(x1+j,y1+i,white);
      } else if(plane[i][j]==6) {
        draw_point(x1+j,y1+i,blue);
      }
    }
  }

}

//戦闘機をけす
void erasedraw_plane(hword x1, hword y1) {
  int i,j;
  for(i=0;i<21;i++) {
    for(j=0;j<21;j++) {
        draw_point(x1+j,y1+i,black);
    }
  }
}


void draw_line(hword xa, hword ya, hword xb, hword yb, hword color) {
  hword* ptr, xc, yc;
  xc = (xa + xb) / 2;
  yc = (ya + yb) / 2;

  draw_point(xc, yc, color);

  if ((xc - xa) >= 1) {
    draw_line(xa, ya, xc, yc, color);
    draw_line(xb, yb, xc, yc, color);
    draw_line(xc, yc, xa, ya, color);
    draw_line(xc, yc, xb, yb, color);
  }
  else {
    return;
  }
}

//グレーを見つけて自分の当たり判定
int search_gray(hword x,hword y) {
  hword *ptr1;
  ptr1=(hword*)VRAM+240*y+x;
  if(*ptr1==gray) {
    return 1;
  } else {
    return 0;
  }
}

//紫の値を見つけて敵の当たり判定
int search_purple(hword x,hword y) {
  hword *ptr3;
  ptr3=(hword*)VRAM+240*y+x;
  if(*ptr3==0x2409) {
    return 1;
  } else {
    return 0;
  }
}

//attack
void lasergun(hword px, hword py, hword color) {
    draw_point(px+10,py-1,color);
    draw_point(px+4,py-1,color);
    draw_point(px+16,py-1,color);
    draw_point(px,py-1,color);
    draw_point(px+20,py-1,color);
}

void display_time(hword val) {
/* 引数を10進数として表示する関数（最大値16ビット）*/
    byte char_data[] = "0123456789";
    byte buf[6];
	hword tmp;
	int i;

	/* 入力値valの桁数設定 */
	i = 3;

	/* 文字列の最後にNULLコード挿入 */
	buf[i+1] = 0;

	/* 最下位桁の文字（10で割った余り）を挿入し，入力値を10で割る */
	for(; i >= 0; i--) {
		buf[i] = char_data[mod(val, 10)];
		val = div(val, 10);
	}

	/* 文字列全体を表示 */
	prints(buf);

	return;
}

//敵の描画
void draw_enemy(hword epx1,hword epy2) {
  int i,j;
  for(j = 0;j<12;j++) {
    for(i = 0;i<13;i++) {
      if(enemy[j][i] == 1) {
        draw_point(epx1+i,epy2+j,purple);
      } else if(enemy[j][i] == 2) {
        draw_point(epx1+i,epy2+j,red);
      }
    }
  }
}

//敵を消す
void erasedraw_enemy(hword epx1,hword epy2,hword color) {
  int i,j;
  for(j = 0;j<12;j++) {
    for(i = 0;i<13;i++) {
      if(enemy[j][i] != 0) {
        draw_point(epx1+i,epy2+j,color);
      }
    }
  }
}


//ボスの描画
void draw_boss(hword x1, hword y1) {
  int i,j;
  for(i=20;i>=0;i--) {
    for(j=20;j>=0;j--) {
      if(plane[i][j]==1) {
        draw_point(x1+21-j,y1+21-i,purple);
      } else if(plane[i][j]==2) {
        draw_point(x1+21-j,y1+21-i,cyan);
      } else if(plane[i][j]==3) {
        draw_point(x1+21-j,y1+21-i,red);
      } else if(plane[i][j]==4) {
        draw_point(x1+21-j,y1+21-i,yellow);
      } else if(plane[i][j]==5) {
        draw_point(x1+21-j,y1+21-i,white);
      } else if(plane[i][j]==6) {
        draw_point(x1+21-j,y1+21-i,blue);
      }
    }
  }

}
//戦闘機をけす
void erasedraw_boss(hword x1, hword y1) {
  int i,j;
  for(i=0;i<21;i++) {
    for(j=0;j<21;j++) {
        draw_point(x1+j+1,y1+i+1,black);
    }
  }
}

//敵の攻撃1
void enemyattack1(hword x,hword y,hword color) {
	draw_point(x,y+11,color);
	draw_point(x+2,y+11,color);
	draw_point(x+4,y+11,color);
	draw_point(x+8,y+11,color);
	draw_point(x+10,y+11,color);
	draw_point(x+12,y+11,color);
}

//敵の攻撃2左
void enemyattack2Left(hword x,hword y,hword color) {
	draw_point(x-4,y+11,color);
	draw_point(x-2,y+11,color);
	draw_point(x,y+11,color);
	draw_point(x+2,y+11,color);
	draw_point(x+4,y+11,color);
	draw_point(x+6,y+11,color);
}

//敵の攻撃右
void enemyattack2Right(hword x,hword y,hword color) {
	draw_point(x+8,y+11,color);
	draw_point(x+10,y+11,color);
	draw_point(x+12,y+11,color);
	draw_point(x+14,y+11,color);
	draw_point(x+16,y+11,color);
	draw_point(x+18,y+11,color);
}
