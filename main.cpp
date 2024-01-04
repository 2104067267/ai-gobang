#include"五子棋.h"

int main(){
    InitWindow(WIDTH*rectangle_size,LENTH*rectangle_size,"first window");
    SetTargetFPS(60);
    chess_map Map;
    for(int i=0;i<WIDTH;i++){
        for(int j=0;j<LENTH;j++){
            Map.map[i][j]=0;
        }
    }
    pair<int,int> center;
    int flag=0,flag1;
    int ind=0,y=0;

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        init_map(Map);
       if( play(Map,black)){
           center=cal_centre(Map,white);
           Map=UCT_search(Map,center,white);
       }
        flag1=check_win(Map);
        flag=is_terminal(Map);
        if(flag1){
            switch(flag)
            {
                case 1:
                    DrawText("black  win!",300,300,50,RED);
                     return 0;
                case 0:
                    DrawText("red  win!",300,300,50,RED);
                    return 0;
            }
        }
        else if(flag){
              DrawText("is full",300,300,50,RED);
              return 0;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

int check_win(chess_map x);////检查游戏是否胜利

int is_terminal(chess_map &x);////检查是否为叶子节点，有无剩余空位置

void init_map(chess_map x);////初始地图

int  play(chess_map &map,int play);////玩家走子

pair<int ,pair<int,int>> key_record(chess_map x,int player);////寻找关键棋形

pair<int,int> cal_centre(chess_map x,int player);////棋子、棋型计算走子中心

#define search_range 3 ////中心搜索范围

#define _depth 3 ////搜索深度

int ab_search( int depth, chess_map x, int player, int a, int b);////剪枝搜索

chess_map UCT_search(chess_map x,pair<int,int> center,int player);////蒙特卡罗树搜索

pair<int,pair<int,int>> predict_win(chess_map x , int player);////预言胜利

int get_score(int &num);////棋形赋分函数

int  calculate_score(chess_map x,int player);////估值函数

int link_evaluate(chess_map x,int player);////连通行估分

int centerlise(chess_map x,int player);////

int __centerlise(int size,int**board,chess_map x,int player,pair<int,int> location);////目数估分

chess_map copy(chess_map x);////复制函数


chess_map copy(chess_map x){
    chess_map y;
    for(int i=0;i<WIDTH;i++){
        for(int j=0;j<LENTH;j++){
            y.map[i][j]=x.map[i][j];
        }
    }
    return y;
}


////赋分函数
int get_score(int& num){
     if(num==1)return 80000;//活四
     if(num<=4&&num>1)return 10000;//死四
     if(num<=6&&num>4)return 1000;//活三
     if(num<=12&&num>6)return 100;//眠三
     if(num<=15&&num>12)return 40;//活二
     if(num<=19&&num>15)return 15;//眠二
}

int link_evaluate(chess_map x,int player){
    int board[WIDTH][LENTH]={0};
    int count=0;
    #define grade 1
    for(int i=0;i<WIDTH;i++){
        for(int j=0;j<LENTH;j++){
            if(x.map[i][j]){
                 int n=i,m=j;
                 board[i][j]=1;
                 while(m<LENTH&&x.map[n][m++]==player){count++;board[n][m]+=1;}
                 n=i;m=j;
                 while(n<WIDTH&&m<LENTH&&x.map[n++][m++]==player){count++;board[n][m]+=1;}
                 n=i;m=j;
                 while(n<WIDTH&&m<LENTH&&x.map[n++][m]==player){count++;board[n][m]+=1;}
                 n=i;m=j;
                 while(n<WIDTH&&m>=0&&x.map[n++][m--]==player){count++;board[n][m]+=1;}\
            }
        }
    }
    for(int i=0;i<WIDTH;i++){
        for(int j=0;j<LENTH;j++){
            while(board[i][j]>1){
                count--;
                board[i][j]--;
            }
        }
    }

    return count*grade;
}


int Is_empty(chess_map x,int player){
        for(int i=0;i<WIDTH;i++){
            for(int j=0;j<LENTH;j++){
                if(!x.map[i][j]){return 0;}
            }
        }
        return 1;
}



int __centerlise(int size,int**board,chess_map x,int player,pair<int,int> location){
        if(board[location.first][location.second]>1){return 0;}
        int count=0,x1,y1;
        ////纵向
        x1=location.first;
        y1=min(location.second+1,LENTH-1);
        if(x.map[x1][y1]==player&& board[x1][y1]!=1){
            board[x1][y1]+=1;
            count+=__centerlise(size,board,x,player, make_pair(x1,y1));
        }
        else if(!x.map[x1][y1]&&board[x1][y1]<1){
               board[x1][y1]+=1;
               count++;
        }
     x1=location.first;
     y1=max(location.second-1,0);
     if(x.map[x1][y1]==player&& board[x1][y1]!=1){
        board[x1][y1]+=1;
        count+=__centerlise(size,board,x,player, make_pair(x1,y1));
      }
     else if(!x.map[x1][y1]&&board[x1][y1]<1){
        board[x1][y1]+=1;
        count++;
     }

    ////横向
    x1=min(location.first+1,WIDTH-1);
    y1=location.second;
    if(x.map[x1][y1]==player&& board[x1][y1]!=1){
        board[x1][y1]+=1;
        count+=__centerlise(size, board, x, player, make_pair(x1, y1));
    }
    else if(!x.map[x1][y1]&&board[x1][y1]<1){
        board[x1][y1]+=1;
        count++;
    }

    x1=max(location.first-1,0);
    y1=location.second;
    if(x.map[x1][y1]==player&& board[x1][y1]!=1){
        board[x1][y1]+=1;
        count+=__centerlise(size, board, x, player, make_pair(x1, y1));
    }
    else if(!x.map[x1][y1]&&board[x1][y1]<1){
        board[x1][y1]+=1;
        count++;
    }

    ////左斜
    x1=min(location.first+1,WIDTH-1);
    y1=min(location.second+1,LENTH-1);
    if(x.map[x1][y1]==player&& board[x1][y1]!=1){
        board[x1][y1]+=1;
        count+=__centerlise(size, board, x, player, make_pair(x1, y1));
    }
    else if(!x.map[x1][y1]&&board[x1][y1]<1){
        board[x1][y1]+=1;
        count++;
    }

    x1=max(location.first-1,0);
    y1=max(location.second-1,0);
    if(x.map[x1][y1]==player&& board[x1][y1]!=1){
        board[x1][y1]+=1;
        count+=__centerlise(size, board, x, player, make_pair(x1, y1));
    }
    else if(!x.map[x1][y1]&&board[x1][y1]<1){
        board[x1][y1]+=1;
        count++;
    }

   ////右斜
    x1=min(location.first+1,WIDTH-1);
    y1=max(location.second-1,0);
    if(x.map[x1][y1]==player&& board[x1][y1]!=1){
        board[x1][y1]+=1;
        count+=__centerlise(size, board, x, player, make_pair(x1, y1));
    }
    else if(!x.map[x1][y1]&&board[x1][y1]<1){
        board[x1][y1]+=1;
        count++;
    }

    x1=max(location.first-1,0);
    y1=min(location.second+1,LENTH-1);
    if(x.map[x1][y1]==player&& board[x1][y1]!=1){
        board[x1][y1]+=1;
        count+=__centerlise(size, board, x, player, make_pair(x1, y1));
    }
    else if(!x.map[x1][y1]&&board[x1][y1]<1){
        board[x1][y1]+=1;
        count++;
    }

    return count;
}


int centerlise(chess_map x,int player){
        int **board=(int**)malloc(sizeof(int)*LENTH*WIDTH);
        int count=0;
        for(int i=0;i<WIDTH;i++){
            for(int j=0;j<LENTH;j++){
                if(x.map[i][j]==player&&board[i][j]==0){
                    count+=__centerlise(LENTH, board, x, player, make_pair(i, j));
                }
            }
        }
          free(board);
        return count/2;
}


int __calculate_score(chess_map x,int player){
    chess_map ans=copy(x);
    /////四个方向依次按照从左到右， 从上到下，从左上到右下，从左下到右上 来检测。
    ////数组遍历检测必须从俩个方向检测，才能涵盖所有情况；
    ////不全,在于不能区分开多种谱图的结合情况，但可效益最大化
    int board[WIDTH][LENTH]={0};

    //记录的分数
    int score=0;

    vector<vector<int>> chess_record={//冲4
            {0,player,player,player,player,0},//1
            //眠四
            {-player,player,player,player,player,0},{0,player,player,player,0,player,0},{0,player,player,0,player,player,0},//4
            //活三
            {0,player,player,player,0},{0,player,player,0,player,0},//6
            //眠三
            {-player,player,player,player,0,0},{-1,player,player,player,0,0},{-player,player,0,player,player,0},//
            {0,player,player,0,0,player,0},{0,player,0,player,0,player,0},{-player,0,player,player,player,0,-1},//12
            //活二
            {0,0,player,player,0,0},{0,player,0,player,0},{0,player,0,0,player},//15
            //眠二
            {-player,player,player,0,0,0},{-1,player,0,player,0,0},{-player,player,0,0,player,0},{player,0,0,0,0,player}//19
    };
    vector<vector<int>>::iterator it;
    vector<vector<int>>::iterator _it;
    vector<vector<int>>::iterator it1,it2,it3,it4;
    ////棋形分值评估
    //黑棋得分
    for(int i=0;i<WIDTH;i++)
    {
        for(int j=0;j<LENTH;j++)
        {    //表示当前为黑子
            if(ans.map[i][j]==player)
            {
                int ind,flag1;
                //横向
                for ( it=chess_record.begin(); it != chess_record.end(); it++) {
                    for(int m=max(0,i-4);m<=min(WIDTH-1,i+4);m++) {
                        flag1=0,ind=0;
                        if (ans.map[m][j] == (*it)[ind]) {
                            int k = m;
                            int flag=1;

                            if(flag) {
                                for (ind = 1; ind < (*it).size()&& m<=min(WIDTH-1,i+4)&&(board[m][j]!=1||it1!=it);m++,ind++) {
                                    if (ans.map[m][j] != (*it)[ind])
                                    {m=k;flag1+=1;break;}
                                }
                                if(ind==(*it).size())flag=0;
                            }

                            if(flag) {
                                for (ind=(*it).size()- 1; ind>=0 && m <= min(WIDTH - 1, i + 4) &&(board[m][j]!=1||it1!=it); m++, ind--) {
                                    if (ans.map[m][j] != (*it)[ind]) {
                                        m = k;
                                        flag1 += 1;
                                        break;
                                    }
                                }
                            }
                        }
                        if(flag1==1){
                            for(;m>m -(*it).size();m--){
                                board[m][j]=1;
                            }
                            _it=it;
                            it1=it;
                            int num=1;
                            //判断棋谱用于赋分
                            for(;_it>chess_record.begin();num++,_it--);
                            score+= get_score(num);
                            break;
                        }
                    }
                }

                //纵向
                for ( it=chess_record.begin(); it != chess_record.end(); it++) {
                    for(int m=max(0,j-4);m<=min(LENTH-1,j+4);m++) {
                        flag1=0,ind=0;
                        if (ans.map[i][m] == (*it)[ind]) {
                            int k = m,flag=1;
                            if(flag){
                                for (ind = 1; ind < (*it).size()&&m<=min(LENTH-1,j+4)&&(board[i][m] != 1);m++,ind++) {
                                    if (ans.map[i][m] != (*it)[ind])
                                    {m=k;flag1+=1;break;}
                                }
                                if(ind==(*it).size())flag=0;
                            }
                            if(flag) {
                                for (ind = (*it).size() - 1;ind>=0&& m <= min(LENTH - 1, j+ 4)&&(board[i][m] != 1); m++, ind--) {
                                    if (ans.map[m][j] != (*it)[ind]) {
                                        m = k;
                                        flag1 += 1;
                                        break;
                                    }
                                }
                            }
                        }
                        if(flag1==1){
                            for(;m>m-(*it).size();m--){
                                board[i][m]=1;
                            }
                            _it=it;
                            it2=it;
                            int num=1;
                            for(;_it>chess_record.begin();num++,_it--);
                            score+= get_score(num);
                            break;
                        }
                    }
                }

                //左上
                for ( it=chess_record.begin(); it != chess_record.end(); it++) {
                    for(int m=max(0,i-4),n=max(0,j-4);m<=min(WIDTH-1,i+4)&&n<=min(LENTH,j+4);m++,n++) {
                        flag1=0,ind=0;
                        if (ans.map[m][n] == (*it)[ind]) {
                            int k1 = m,k2=n,flag=1;

                            if(flag){
                                for (ind = 1; ind < (*it).size()&&m<=min(WIDTH-1,i+4)&&n<=min(LENTH,j+4)&&(board[m][n]!=1);m++,n++,ind++) {
                                    if (ans.map[m][j] != (*it)[ind])
                                    {m=k1;n=k2;flag1+=1;break;}
                                }
                                if(ind==(*it).size())flag=0;
                            }

                            if(flag) {
                                for (ind = (*it).size() - 1;ind>=0&& m <= min(WIDTH - 1, i+ 4)&&n<=min(LENTH-1,j+4) &&(board[m][n]!=1); m++,n++, ind--) {
                                    if (ans.map[m][j] != (*it)[ind]) {
                                        m = k1;n=k2;
                                        flag1 += 1;
                                        break;
                                    }
                                }
                            }
                        }
                        if(flag1==1){
                            for(;m>m-(*it).size();m--,n--){
                                board[m][n]=1;
                            }
                            _it=it;
                            it3=it;
                            int num=1;
                            for(;_it>chess_record.begin();num++,_it--);
                            score+= get_score(num);
                            break;
                        }
                    }
                }

                //左下
                for ( it=chess_record.begin(); it != chess_record.end(); it++) {
                    for(int m=max(0,i-4),n=min(LENTH-1,j+4);m<=min(WIDTH-1,i+4)&&n>=max(0,j-4);m++,n--) {
                        flag1=0,ind=0;
                        if (ans.map[m][n] == (*it)[ind]) {
                            int k1 = m,k2=n,flag=1;
                            if(flag) {
                                for (ind = 1;ind < (*it).size() && m <= min(WIDTH - 1, i + 4) && n >= max(0, j - 4) &&(board[m][n] != 1); m++, n--, ind++) {
                                    if (ans.map[m][j] != (*it)[ind]) {
                                        m = k1;
                                        n = k2;
                                        flag1 += 1;
                                        break;
                                    }
                                }
                                if(ind==(*it).size())flag=0;
                            }
                            if(flag){
                                for (ind =(*it).size()-1; ind>=0&&m<=min(WIDTH-1,i+4)&&n>=max(0,j-4)&&(board[m][n] != 1);m++,n--,ind--) {
                                    if (ans.map[m][j] != (*it)[ind])
                                    {m=k1;n=k2;flag1+=1;break;}
                                }
                            }

                        }
                        if(flag1==1){
                            for(;m>m-(*it).size();m--,n++){
                                board[m][n]=1;
                            }
                            _it=it;
                            it4=it;
                            int num=1;
                            for(;_it>chess_record.begin();num++,_it--);
                            score+= get_score(num);
                            break;
                        }
                    }
                }
            }
        }
    }
    for(it=chess_record.begin();it!=chess_record.end();it++)
    {
        (*it).clear();
    }
    chess_record.clear();
    ////己方的棋形，连通性，活度和对手的活度
    return  score+link_evaluate(x,player) +centerlise(x,player)- centerlise(x,-player);
}


////估值函数
 int  calculate_score(chess_map x,int player){
     if(is_terminal(x))return 0;
     if(check_win(x))return 400000;
     pair<int,pair<int,int>> _k=predict_win(x,player);
     if(_k.first){return 400000;}
          int score_own=0,score_enemy=0;
          score_own= __calculate_score(x,player);
          score_enemy= __calculate_score(x,-player);
          ////判断防守还收进攻
          if(score_own+600<=score_enemy){return -score_enemy;}
          else{return score_own;}
}

int ab_search(int depth, chess_map x, int player, int a, int b){
        if(depth==0||a>=b){return calculate_score(x,player);}
      children ans_child;
      pair<int,int>center= cal_centre(x,player);
    /////计算搜索坐标范围
    int x_min=max(0,center.first-search_range);
    int x_max=min(WIDTH-1,center.first+search_range);
    int y_min=max(0,center.second-search_range);
    int y_max=min(LENTH-1,center.second+search_range);
    ////第一层遍历；
    ////生成树
    chess_map ans=copy(x);
    mp[ans]=ans_child;
    chess_map _ans;
    ////选择
    for(int i=x_min;i<=x_max;i++){
        for(int j=y_min;j<=y_max;j++){
            ////当前位置为空，则进行模拟；
            if(!ans.map[i][j]){
                _ans=ans;
                _ans.map[i][j]=player;
                mp[ans].vec.push_back(_ans);
            }
        }
    }
    int score=0;
    int best_score=-10000000;
    auto it=mp[ans].vec.begin();

   if(player==1) {
       for (; it != mp[ans].vec.end(); it++) {
           score= ab_search(depth - 1, *it, -player, a, b);
           if(score>best_score){
               best_score=score;
               father[ans]=*it;
           }
           a=max(a,score);
           if (a>b) { break;}
       }
       return best_score;
   }
   else {
       for (; it != mp[ans].vec.end(); it++) {
           score = ab_search(depth - 1, *it, -player, a, b);
           if(score<best_score){
               best_score=score;
               father[ans]=*it;
           }
            b=min(b,score);
           if (a>b) { break; }
       }

       return best_score;
   }
}

////蒙特喀罗树实现
chess_map UCT_search(chess_map x,pair<int,int> center,int player){
    if(is_terminal(x))return x;
    ////预言胜利
    pair<int,pair<int,int>> location=predict_win(x,player);
    if(location.first) {x.map[location.second.first][location.second.second]=player;return x;}
    children ans_child;
    /////计算搜索坐标范围
    int x_min=max(0,center.first-search_range);
    int x_max=min(WIDTH-1,center.first+search_range);
    int y_min=max(0,center.second-search_range);
    int y_max=min(LENTH-1,center.second+search_range);
    ////第一层遍历；
   ////生成树
    chess_map ans=copy(x);
    mp[ans]=ans_child;
    chess_map _ans;
    ////选择
   for(int i=x_min;i<=x_max;i++){
       for(int j=y_min;j<=y_max;j++){
           ////当前位置为空，则进行模拟；
           if(!ans.map[i][j]){
                 _ans=ans;
                _ans.map[i][j]=player;
                ////进行alpha_beta剪枝
                ab_search(_depth, _ans, -player, -1000000, 1000000);
                mp[ans].vec.push_back(_ans);
           }
       }
   }
    auto it=mp[ans].vec.begin();
    auto _it=it;
    chess_map __it;
    ////minmax估分，对各层棋盘进行权值估分 获得最佳棋盘
    int score_1,score_total=-1000000,score_cur;
    it=mp[x].vec.begin();
    for (; it != mp[x].vec.end(); it++)
    {
        //当前层进行权重估分
        score_1=calculate_score(*it, player)/2;
        int i=_depth;
        __it=*it;
        while(i--){
            score_1+=calculate_score(father[__it],player*=-1)/3;
            __it=father[__it];
        }
        if(score_total<score_cur) {score_total=score_cur;_it=it;}
    }
    chess_map ans2=copy(*_it);
    mp[x].vec.clear();
    return ans2;
}


int is_terminal(chess_map &x){
    for(int i=0;i<WIDTH;i++){
        for(int j=0;j<LENTH;j++){
            if(!x.map[i][j])return 0;
        }
    }
    return 1;
}

////寻找局部落子中心
pair<int ,pair<int,int>> key_record(chess_map x,int player){
    chess_map ans=copy(x);
    /////四个方向依次按照从左到右， 从上到下，从左上到右下，从左下到右上 来检测。
    ////数组遍历检测必须从俩个方向检测，才能涵盖所有情况；
    ////不全,在于不能区分开多种谱图的结合情况，但可效益最大化
    int board[WIDTH][LENTH]={0};
    int score=0;
    vector<vector<int>> chess_record={//冲4
            {0,player,player,player,player,0},//1
            //眠四
            {-player,player,player,player,player,0},{0,player,player,player,0,player,0},{0,player,player,0,player,player,0},//4
            //活三
            {0,player,player,player,0},{0,player,player,0,player,0}//6
    };
    vector<vector<int>>::iterator it;
    vector<vector<int>>::iterator _it;
    vector<vector<int>>::iterator it1,it2,it3,it4;
    ////棋形分值评估
    //黑棋得分
    for(int i=0;i<WIDTH;i++)
    {
        for(int j=0;j<LENTH;j++)
        {    //表示当前为黑子
            if(ans.map[i][j]==player)
            {
                int ind,flag1;
                //横向
                  for ( it=chess_record.begin(); it != chess_record.end(); it++) {
                    for(int m=max(0,i-4);m<=min(WIDTH-1,i+4);m++) {
                        flag1=0,ind=0;
                        if (ans.map[m][j] == (*it)[ind]) {
                            int k = m;
                            int flag=1;

                            if(flag) {
                                for (ind = 1; ind < (*it).size()&& m<=min(WIDTH-1,i+4)&&(board[m][j]!=1||it1!=it);m++,ind++) {
                                    if (ans.map[m][j] != (*it)[ind])
                                    {m=k;flag1+=1;break;}
                                }
                                if(ind==(*it).size())flag=0;
                            }

                            if(flag) {
                                for (ind=(*it).size()- 1; ind>=0 && m <= min(WIDTH - 1, i + 4) &&(board[m][j]!=1||it1!=it); m++, ind--) {
                                    if (ans.map[m][j] != (*it)[ind]) {
                                        m = k;
                                        flag1 += 1;
                                        break;
                                    }
                                }
                            }
                        }
                        if(flag1==1){
                            for(;m>m -(*it).size();m--){
                                board[m][j]=1;
                            }
                            _it=it;
                            it1=it;
                            int num=1;
                            //判断棋谱用于赋分
                            for(;_it>chess_record.begin();num++,_it--)

                           return make_pair(1,make_pair(m,j));
                        }
                    }

                //纵向
                    for(int m=max(0,j-4);m<=min(LENTH-1,j+4);m++) {
                        flag1=0,ind=0;
                        if (ans.map[i][m] == (*it)[ind]) {
                            int k = m,flag=1;
                            if(flag){
                                for (ind = 1; ind < (*it).size()&&m<=min(LENTH-1,j+4)&&(board[i][m] != 1);m++,ind++) {
                                    if (ans.map[i][m] != (*it)[ind])
                                    {m=k;flag1+=1;break;}
                                }
                                if(ind==(*it).size())flag=0;
                            }
                            if(flag) {
                                for (ind = (*it).size() - 1;ind>=0&& m <= min(LENTH - 1, j+ 4)&&(board[i][m] != 1); m++, ind--) {
                                    if (ans.map[m][j] != (*it)[ind]) {
                                        m = k;
                                        flag1 += 1;
                                        break;
                                    }
                                }
                            }
                        }
                        if(flag1==1){
                            for(;m>m-(*it).size();m--){
                                board[i][m]=1;
                            }
                            _it=it;
                            it2=it;
                            int num=1;
                            for(;_it>chess_record.begin();num++,_it--);
                            return make_pair(1,make_pair(i,m));
                        }
                    }


                //左上
                    for(int m=max(0,i-4),n=max(0,j-4);m<=min(WIDTH-1,i+4)&&n<=min(LENTH,j+4);m++,n++) {
                        flag1=0,ind=0;
                        if (ans.map[m][n] == (*it)[ind]) {
                            int k1 = m,k2=n,flag=1;

                            if(flag){
                                for (ind = 1; ind < (*it).size()&&m<=min(WIDTH-1,i+4)&&n<=min(LENTH,j+4)&&(board[m][n]!=1);m++,n++,ind++) {
                                    if (ans.map[m][j] != (*it)[ind])
                                    {m=k1;n=k2;flag1+=1;break;}
                                }
                                if(ind==(*it).size())flag=0;
                            }

                            if(flag) {
                                for (ind = (*it).size() - 1;ind>=0&& m <= min(WIDTH - 1, i+ 4)&&n<=min(LENTH-1,j+4) &&(board[m][n]!=1); m++,n++, ind--) {
                                    if (ans.map[m][j] != (*it)[ind]) {
                                        m = k1;n=k2;
                                        flag1 += 1;
                                        break;
                                    }
                                }
                            }
                        }
                        if(flag1==1){
                            for(;m>m-(*it).size();m--,n--){
                                board[m][n]=1;
                            }
                            _it=it;
                            it3=it;
                            int num=1;
                            for(;_it>chess_record.begin();num++,_it--);
                            return make_pair(1,make_pair(m,n));
                        }
                    }


                      //左下
                    for(int m=max(0,i-4),n=min(LENTH-1,j+4);m<=min(WIDTH-1,i+4)&&n>=max(0,j-4);m++,n--) {
                        flag1=0,ind=0;
                        if (ans.map[m][n] == (*it)[ind]) {
                            int k1 = m,k2=n,flag=1;
                            if(flag) {
                                for (ind = 1;ind < (*it).size() && m <= min(WIDTH - 1, i + 4) && n >= max(0, j - 4) &&(board[m][n] != 1); m++, n--, ind++) {
                                    if (ans.map[m][j] != (*it)[ind]) {
                                        m = k1;
                                        n = k2;
                                        flag1 += 1;
                                        break;
                                    }
                                }
                                if(ind==(*it).size())flag=0;
                            }
                            if(flag){
                                for (ind =(*it).size()-1; ind>=0&&m<=min(WIDTH-1,i+4)&&n>=max(0,j-4)&&(board[m][n] != 1);m++,n--,ind--) {
                                    if (ans.map[m][j] != (*it)[ind])
                                    {m=k1;n=k2;flag1+=1;break;}
                                }
                            }

                        }
                        if(flag1==1){
                            for(;m>m-(*it).size();m--,n++){
                                board[m][n]=1;
                            }
                            _it=it;
                            it4=it;
                            int num=1;
                            for(;_it>chess_record.begin();num++,_it--);
                            return make_pair(1,make_pair(m,n));
                        }
                    }
                }
            }
        }
    }
    for(it=chess_record.begin();it!=chess_record.end();it++)
    {
        (*it).clear();
    }
    chess_record.clear();
    ////己方的棋形，连通性，活度和对手的活度
    return  make_pair(0,make_pair(0,0));
}

  pair<int,int> cal_centre(chess_map x,int player){
       ////以每个棋子横纵坐标的均值为搜索中心
      pair<int,pair<int,int>> own_center=key_record(x,player);
      pair<int,pair<int,int>> enemy_center=key_record(x,-player);
      ////攻
      if(own_center.first){return own_center.second;}
      ////防
      else if(enemy_center.first){return enemy_center.second;}

    pair<int,int> center;
    int cnt=0,p1=0,p2=0;
    for(int i=0;i<WIDTH;i++)
    {
        for(int j=0;j<LENTH;j++)
        {
            if(x.map[i][j]==player){
                cnt++;
                p1+=i;
                p2+=j;
            }
        }
    }
    ////控制初始首棋于中心位置
    if(!cnt){return make_pair(WIDTH/2,LENTH/2);}
    p1=max(0,p1/cnt);
    p2=max(0,p2/cnt);
    center.first=p1;
    center.second=p2;
    return center;
}


void init_map(chess_map x) {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < LENTH; j++) {
            DrawLine(0, i * rectangle_size, WIDTH * rectangle_size, i * rectangle_size, BLACK);
            DrawLine(j * rectangle_size, 0, j * rectangle_size, WIDTH * rectangle_size, BLACK);
            //DrawRectangle(i*rectangle_size+rectangle_size/2,j*rectangle_size+rectangle_size/2,rectangle_size,rectangle_size,YELLOW);
            if (x.map[i][j] == black) {
                DrawCircle(i * rectangle_size + rectangle_size / 2, j * rectangle_size + rectangle_size / 2,
                           rectangle_size / 2, BLACK);
            }
            if (x.map[i][j] == white) {
                DrawCircle(i * rectangle_size + rectangle_size / 2, j * rectangle_size + rectangle_size / 2,
                           rectangle_size / 2, RED);
            }
        }
    }
    DrawLine(0, WIDTH * rectangle_size, WIDTH * rectangle_size, WIDTH * rectangle_size, BLACK);
    DrawLine(LENTH * rectangle_size, 0, LENTH * rectangle_size, LENTH * rectangle_size, BLACK);
    return;
}


int  play(chess_map &map,int play){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 a = GetMousePosition();
            int x = (int) a.x / rectangle_size;
            int y = (int) a.y / rectangle_size;
            if (x < WIDTH && x >= 0 && y < LENTH && y >= 0&&map.map[x][y]==EMPTY) {
                map.map[x][y] = play;
                return 1;
            }
        }
    return 0;
}


int check_win(chess_map x) {
    int count,i,j;
    // 检查行、列和对角线是否有五子连珠的情况
    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (x.map[i][j] != EMPTY) {
                count = 1;
                //横向
                while (count < 5) {
                    if (i - count >= 0 && x.map[i - count][j] == x.map[i][j]) {
                        count++;
                        continue;
                    }else{count=1;break;}
                }
                    //纵向
                while (count < 5) {
                    if (j - count >= 0 && x.map[i][j - count] == x.map[i][j]) {
                        count++;
                        continue;
                    }
                    else{count=1;break;}
                }
                //左上
                while (count < 5) {
                    if (j + count < WIDTH && i - count >= 0 && x.map[i - count][j + count] == x.map[i][j]) {
                        count++;
                        continue;
                    }
                    else{count=1;break;}
                }
                //左上
                while (count < 5) {
                    if (j - count >= 0 && i - count >= 0 && x.map[i - count][j - count] == x.map[i][j]) {
                        count++;
                        continue;
                    }
                    else{count=1;break;}
                }

                if (count == 5) {  // 五子连珠了，返回1表示游戏结束
                   if(x.map[i][j]==1)return 1;
                   else if(x.map[i][j]==-1) return -1;
                }
            }
        }
    }
    return 0;
}


//预言胜利
pair<int,pair<int,int>> predict_win(chess_map x , int player){
    pair<int,pair<int,int>> location;
    location.first=0;
    location.second.first=0;
    location.second.second=0;
    chess_map ans=copy(x);

    for(int i=0; i < WIDTH;i++)
    {
        for(int j=0;j<LENTH;j++)
        {
            if(!ans.map[i][j])
            {
                ans.map[i][j]=player;
                int flag=check_win(ans);
                if(flag)
                {    location.first=1;
                     location.second.first=i;
                    location.second.second=j;
                     ans.map[i][j]=EMPTY;
                     return location;
                }
                ans.map[i][j]=EMPTY;
            }
        }
    }
    return location;
}