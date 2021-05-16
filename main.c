#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define rep(i, n) for(int i = 0; i < n; ++i)
#define rrep(i, n) for(int i = n-1; i >= 0; --i)

void print_legend(unsigned int f) {
    // 上方向へ動かすことが可能かどうか
    if(f & (1 << 0)) {
        printf("      'i'\r\n");
        printf("       ↑\r\n");
    } else {
        printf("\r\n");
        printf("\r\n");
    }
    // 左方向へ動かすことが可能かどうか
    if(f & (1 << 2)) {
        printf("'j'← Press");
    } else {
        printf("     Press");
    }
    // 右方向へ動かすことが可能かどうか
    if(f & (1 << 3)) {
        printf(" →'k'\r\n");
    } else {
        printf("\r\n");
    }
    // 下方向へ動かすことが可能かどうか
    if(f & (1 << 1)) {
        printf("       ↓\r\n");
        printf("      'm'\r\n");
    } else {
        printf("\r\n");
        printf("\r\n");
    }
    // シャッフル可能かどうか
    if(f & (1 << 4)) {
        printf("Press 's' to shuffle the board(use 5 turns)\r\n");
    } else {
        printf("\r\n");
    }
    // 盤面が隠されているかどうか
    if(f & (1 << 5)) {
        printf("Press 'g' to show the board.\r\n");
    } else {
        printf("Press 'h' to hide the board.\r\n");
    }
    // ゲーム終了キー
    printf("Press '.' to close\r\n");
}

void print_board(int *v, int n, int turns, int score, unsigned int flag) {
    rep(i, 10*n+1) {
        printf("-");
    }
    printf("\r\n");
    rep(i, n) {
        printf("|");
        rep(j, n) {
            if(flag & (1 << 5)) {
                printf("  ?????  |");
            } else if(v[i * n + j] == 0) {
                printf("         |");
            } else {
                printf(" %7d |", v[i * n + j]);
            }
        }
        printf("\r\n");
        rep(i, 10*n+1) {
            printf("-");
        }
        printf("\r\n");
    }

    // 現在の記録表示
    printf("Remaining turns: %d\r\n", turns);
    printf("Your score: %d\r\n", score);
}

void random_generate(int *v, int n, int gamemode) {
    srand(time(NULL)); // 乱数生成を初期化
    // RAND_MAX = 2147483647
    int is_generated = 0;
    while(is_generated == 0) {
        rep(i, n) {
            rep(j, n) {
                if(v[i * n + j] == 0) {
                    int random = rand();
                    if(random < RAND_MAX * 0.001) {
                        if(gamemode) v[i*n+j] = -4;
                        else v[i*n+j] = 4; // たまに出る4
                        is_generated = 1;
                        break;
                    } else if(random < RAND_MAX * 0.02) {
                        if(gamemode) v[i*n+j] = -2;
                        else v[i*n+j] = 2;
                        is_generated = 1;
                        break;
                    }
                }
            }
            if(is_generated) break;
        }
    }
}

int move_check_u(int *v, int n) {
    int r = 0;
    rep(i, n){
        int tmp = v[i];
        rep(j, n){
            if(v[j*n+i] == 0) {
                // 0があって動かせる場合を判定
                rep(k, n-j) {
                    if(v[(j+k)*n+i] != 0) {
                        r = 1;
                        break;
                    }
                }
                if(r) break;
            } else if(j > 0) {
                // 0を飛ばして合体できるか判定
                if(v[j*n+i] == tmp) {
                    r = 1;
                    break;
                } else {
                    tmp = v[j*n+i];
                }
            }
        }
        if(r) break;
    }
    return r;
}

int move_check_d(int *v, int n) {
    int r = 0;
    rep(i, n){
        int tmp = v[(n-1)*n+i];
        rrep(j, n){
            if(v[j*n+i] == 0) {
                // 0があって動かせる場合を判定
                rrep(k, j) {
                    if(v[k*n+i] != 0) {
                        r = 1;
                        break;
                    }
                }
                if(r) break;
            } else if(j < n-1) {
                // 0を飛ばして合体できるか判定
                if(v[j*n+i] == tmp) {
                    r = 1;
                    break;
                } else {
                    tmp = v[j*n+i];
                }
            }
        }
        if(r) break;
    }
    return r;
}

int move_check_l(int *v, int n) {
    int r = 0;
    rep(i, n){
        int tmp = v[i*n];
        rep(j, n){
            if(v[i*n+j] == 0) {
                // 0があって動かせる場合を判定
                rep(k, n-j) {
                    if(v[i*n+j+k] != 0) {
                        r = 1;
                        break;
                    }
                    if(r) break;
                }
            } else if(j > 0) {
                // 0を飛ばして合体できるか判定
                if(v[i*n+j] == tmp) {
                    r = 1;
                    break;
                } else {
                    tmp = v[i*n+j];
                }
            }
        }
        if(r) break;
    }
    return r;
}

int move_check_r(int *v, int n) {
    int r = 0;
    rep(i, n){
        int tmp = v[i*n+n-1];
        rrep(j, n){
            if(v[i*n+j] == 0) {
                // 0があって動かせる場合を判定
                rrep(k, j) {
                    if(v[i*n+k] != 0) {
                        r = 1;
                        break;
                    }
                    if(r) break;
                }
            } else if(j < n-1) {
                // 0を飛ばして合体できるか判定
                if(v[i*n+j] == tmp) {
                    r = 1;
                    break;
                } else {
                    tmp = v[i*n+j];
                }
            }
        }
        if(r) break;
    }
    return r;
}

void move_u(int *v, int n, int *s) {
    int tmpv[n*n];
    rep(i, n) {
        rep(j, n) {
            tmpv[i*n+j] = 0;
        }
    }
    rep(i, n) {
        int tmp = 0, can_merge = 1; // tmpはtmpv[tmp*n+i]の位置、can_mergeは合成できるかどうか(3つ以上合成することになってないか)のフラグ
        rep(j, n){
            if(v[j*n+i] == 0){
                continue;
            } else {
                if(j == 0) {
                    tmpv[tmp*n+i] = v[j*n+i];
                } else {
                    if(can_merge && tmpv[tmp*n+i] == v[j*n+i]) {
                        // 合成できる場合
                        tmpv[tmp*n+i] *= 2;
                        *s += tmpv[tmp*n+i];
                        can_merge = 0;
                    } else {
                        // 合成できない場合
                        if(tmpv[tmp*n+i] != 0) ++tmp; // 0の場合は詰める
                        tmpv[tmp*n+i] = v[j*n+i];
                        can_merge = 1;
                    }
                }
            }
        }
    }
    rep(i, n) {
        rep(j, n) {
            v[i*n+j] = tmpv[i*n+j]; // 更新
        }
    }
}

void move_d(int *v, int n, int *s) {
    int tmpv[n*n];
    rep(i, n) {
        rep(j, n) {
            tmpv[i*n+j] = 0;
        }
    }
    rep(i, n) {
        int tmp = n-1, can_merge = 1; // tmpはtmpv[tmp*n+i]の位置、can_mergeは合成できるかどうか(3つ以上合成することになってないか)のフラグ
        rrep(j, n){
            if(v[j*n+i] == 0){
                continue;
            } else {
                if(j == n-1) {
                    tmpv[tmp*n+i] = v[j*n+i];
                } else {
                    if(can_merge && tmpv[tmp*n+i] == v[j*n+i]) {
                        // 合成できる場合
                        tmpv[tmp*n+i] *= 2;
                        *s += tmpv[tmp*n+i];
                        can_merge = 0;
                    } else {
                        // 合成できない場合
                        if(tmpv[tmp*n+i] != 0) --tmp; // 0の場合は詰める
                        tmpv[tmp*n+i] = v[j*n+i];
                        can_merge = 1;
                    }
                }
            }
        }
    }
    rep(i, n) {
        rep(j, n) {
            v[i*n+j] = tmpv[i*n+j]; // 更新
        }
    }
}

void move_l(int *v, int n, int *s) {
    int tmpv[n*n];
    rep(i, n) {
        rep(j, n) {
            tmpv[i*n+j] = 0;
        }
    }
    rep(i, n) {
        int tmp = 0, can_merge = 1; // tmpはtmpv[tmp*n+i]の位置、can_mergeは合成できるかどうか(3つ以上合成することになってないか)のフラグ
        rep(j, n){
            if(v[i*n+j] == 0){
                continue;
            } else {
                if(j == 0) {
                    tmpv[i*n+tmp] = v[i*n+j];
                } else {
                    if(can_merge && tmpv[i*n+tmp] == v[i*n+j]) {
                        // 合成できる場合
                        tmpv[i*n+tmp] *= 2;
                        *s += tmpv[i*n+tmp];
                        can_merge = 0;
                    } else {
                        // 合成できない場合
                        if(tmpv[i*n+tmp] != 0) ++tmp; // 0の場合は詰める
                        tmpv[i*n+tmp] = v[i*n+j];
                        can_merge = 1;
                    }
                }
            }
        }
    }
    rep(i, n) {
        rep(j, n) {
            v[i*n+j] = tmpv[i*n+j]; // 更新
        }
    }
}

void move_r(int *v, int n, int *s) {
    int tmpv[n*n];
    rep(i, n) {
        rep(j, n) {
            tmpv[i*n+j] = 0;
        }
    }
    rep(i, n) {
        int tmp = n-1, can_merge = 1; // tmpはtmpv[tmp*n+i]の位置、can_mergeは合成できるかどうか(3つ以上合成することになってないか)のフラグ
        rrep(j, n){
            if(v[i*n+j] == 0){
                continue;
            } else {
                if(j == n-1) {
                    tmpv[i*n+tmp] = v[i*n+tmp];
                } else {
                    if(can_merge && tmpv[i*n+tmp] == v[i*n+j]) {
                        // 合成できる場合
                        tmpv[i*n+tmp] *= 2;
                        *s += tmpv[i*n+tmp];
                        can_merge = 0;
                    } else {
                        // 合成できない場合
                        if(tmpv[i*n+tmp] != 0) --tmp; // 0の場合は詰める
                        tmpv[i*n+tmp] = v[i*n+j];
                        can_merge = 1;
                    }
                }
            }
        }
    }
    rep(i, n) {
        rep(j, n) {
            v[i*n+j] = tmpv[i*n+j]; // 更新
        }
    }
}

void shuffle(int *v, int n) {
    int tmpv[n*n];
    rep(i, n) {
        rep(j, n) {
            tmpv[i*n+j] = -1;
        }
    }
    rep(i, n) {
        rep(j, n) {
            srand(time(NULL)); // 乱数生成を初期化
            // RAND_MAX = 2147483647
            int is_shuffled = 0;
            while(is_shuffled == 0) {
                rep(k, n) {
                    rep(l, n) {
                        if(tmpv[k*n+l] == -1) {
                            int random = rand();
                            if(random < RAND_MAX * 0.01) {
                                tmpv[k*n+l] = v[i*n+j];
                                is_shuffled = 1;
                                break;
                            }
                        }
                    }
                    if(is_shuffled) break;
                }
            }
        }
    }
    rep(i, n) {
        rep(j, n) {
            v[i*n+j] = tmpv[i*n+j];
        }
    }
}

int board_max(int *v, int n) {
    int m = -99999999;
    rep(i, n) {
        if(m < v[i]) {
            m = v[i];
        }
    }
    return m;
}


int main (int argc, char *argv[]) {
    system("/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる

    int board_size = -1, turns = 0, gamemode = -1;
    // モードと盤面サイズとターン数の入力
    while(gamemode < 0) {
        char tmp_mode;
        printf("Press number key to choose game mode.\r\n");
        printf("0 : normal mode\r\n");
        printf("1 : challenge mode\r\n");
        while((tmp_mode = getchar())) {
            if(tmp_mode == '0' || tmp_mode == '1') {
                gamemode = tmp_mode - '0';
                system("clear");
                break;
            } else {
                system("clear");
                printf("You pressed wrong key.\r\n");
                break;
            }
        }
    }
    printf("You chose %d.\r\n", gamemode); // ゲームモードの確認表示
    while(board_size < 0) {
        char tmp_size;
        printf("Press number key to choose board size (2 ~ 6).\r\n");
        while((tmp_size = getchar())) {
            if(tmp_size > '0'+1 && tmp_size < '0'+7) {
                board_size = (int)(tmp_size - '0');
                system("clear");
                break;
            } else {
                system("clear");
                printf("You pressed wrong key.\r\n");
                break;
            }
        }
    }
    printf("You chose %d.\r\n", board_size); // 盤面サイズの確認表示
    system("/bin/stty cooked");  // 後始末
    while(turns <= 0) {
        printf("Press number key to choose number of turns & press Enter key.\r\n");
        int tmp_turns = 0;
        scanf("%d", &tmp_turns);
        if(tmp_turns > 0) {
            turns = tmp_turns;
            break;
        }
        scanf("%*[^\n]");
        system("clear");
        printf("You pressed wrong key.\r\n");
    }

    system("/bin/stty raw onlcr");  // enterを押さなくてもキー入力を受け付けるようになる

    char c = 0;
    int score = 0;
    if(gamemode) score = 1000; // チャレンジモードは減算モード

    // 初期盤面生成
    int board[board_size * board_size];
    rep(i, board_size) {
        rep(j, board_size) {
            board[i * board_size + j] = 0;
        }
    }
    rep(i, board_size-1) random_generate(board, board_size, gamemode); // 初期配置

    int max_number = board_max(board, board_size*board_size);
    unsigned int flags = 16; // 盤面の状態を表すフラグ  2^0の位から順に、u(0),d(1),l(2),r(3)の移動可能性、シャッフル可能性(4)、盤面が隠されているかどうか(5)
    if(move_check_u(board, board_size)) flags += 1 << 0;
    if(move_check_l(board, board_size)) flags += 1 << 2;
    if(move_check_r(board, board_size)) flags += 1 << 3;
    if(move_check_d(board, board_size)) flags += 1 << 1; 

    print_legend(flags);
    print_board(board, board_size, turns, score, flags);

    // 実際のプレイヤーの入力に対する動作
    while((c = getchar()) != '.') {
        system("clear");
        unsigned int tmp_flags = 0; // 入力に対する反応のフラグ  2^0の位から順に、移動不可(0)、シャッフル不可(1)、入力ミス(2)
        if(c == 'i') {
            if(flags & (1 << 0)) {
                move_u(board, board_size, &score);
                random_generate(board, board_size, gamemode);
                --turns;
            } else {
                tmp_flags += 1 << 0;
            }
        } else if(c == 'j') {
            if(flags & (1 << 2)) {
                move_l(board, board_size, &score);
                random_generate(board, board_size, gamemode);
                --turns;
            } else {
                tmp_flags += 1 << 0;
            }
        } else if(c == 'k') {
            if(flags & (1 << 3)) {
                move_r(board, board_size, &score);
                random_generate(board, board_size, gamemode);
                --turns;
            } else {
                tmp_flags += 1 << 0;
            }
        } else if(c == 'm') {
            if(flags & (1 << 1)) {
                move_d(board, board_size, &score);
                random_generate(board, board_size, gamemode);
                --turns;
            } else {
                tmp_flags += 1 << 0;
            }
        } else if(c == 's') {
            if(turns > 4) {
                shuffle(board, board_size);
                turns -= 5;
            } else {
                tmp_flags += 1 << 0;
                tmp_flags += 1 << 1;
            }
        } else if(c == 'h' && !(flags & (1 << 5))) {
            flags += 1 << 5;
        } else if(c == 'g' && (flags & (1 << 5))) {
            flags -= 1 << 5;
        } else {
            tmp_flags += 1 << 0;
            tmp_flags += 1 << 2;
        }

        // 盤面が変更されたら動かせるかどうかの判定をする
        if(!(tmp_flags & 1 << 0)) {
            flags /= 16; flags *= 16; // 各移動可能フラグを0リセットした後、それぞれ足していく
            if(move_check_u(board, board_size)) flags += 1 << 0;
            if(move_check_l(board, board_size)) flags += 1 << 2;
            if(move_check_r(board, board_size)) flags += 1 << 3;
            if(move_check_d(board, board_size)) flags += 1 << 1;
            if((flags & (1 << 4)) && turns < 5) flags -= 1 << 4;
            max_number = board_max(board, board_size*board_size);
        }

        // 入力による操作
        print_legend(flags);
        print_board(board, board_size, turns, score, flags);
        if(c != '\n') printf("You pressed '%c'\r\n", c);
        if(tmp_flags & (1 << 2) && c != '\n') {
            printf("You pressed wrong key.\r\n");
        } else if(tmp_flags & (1 << 1) && c != '\n') {
            printf("You can\'t shuffle.\r\n");
        } else if(tmp_flags & (1 << 0) && c != '\n') {
            printf("You can\'t move.\r\n"); // 動かせない場合の表示
        }

        // ゲームクリア判定
        if(max_number > (1 << 10)) {
            printf("Clear!!! You could make 2048.\r\n");
            printf("You can continue the game and make bigger number.\r\n");
        }
 
        // ゲーム終了判定
        if(turns <= 0) {
            // ターン数が0
            break;
        }
        if(flags % 16 == 0) {
            // 各方向の移動可能性が全て0
            break;
        }
        if(gamemode && score <= 0) {
            // チャレンジモードでスコアが0以下になってしまった時
            break;
        }
    }

    // ゲーム終了
    system("clear");
    printf("Game Over\r\n\r\n");
    print_board(board, board_size, turns, score, 0);
    if(max_number > (1 << 10)) {
        printf("Clear!!! You could make 2048.\r\n");
    }
    if(gamemode && flags % 16 == 0){
        printf("Clear!!! You could finish challenge mode.\r\n");
    }
    system("/bin/stty cooked");  // 後始末

    return 0;
}