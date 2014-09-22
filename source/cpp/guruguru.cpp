// WH/2,WH/2 を中心としたぐるぐる。
// まんなかは1
vector<vector<int>> guruguru(int N){
    const int WH = sqrt(N) + 2;
    vector<vector<int>> field(WH,vector<int>(WH,N + 100));
    {
        int cx=WH/2,cy=WH/2;
        field[cy][cx] = 1;
        int c = 2;
        for(int i=0;;i++){
            int sx = cx + i+1;
            int sy = cy + i;
            for(int j=0;j<2*i+2;j++){
                field[sy-j][sx] = c;
                c++;
                if(c >= MAXN) goto end;
            }
            vector<int> dx = {-1,0,1};
            vector<int> dy = {0,1,0};
            // left
            int ny = sy-2*i-1;
            int nx = sx;
            for(int k=0;k<3;k++){
                for(int j=0;j<2*(i+1);j++){
                    ny = ny + dy[k];
                    nx = nx + dx[k];
                    field[ny][nx] = c;
                    c++;
                    if(c >= MAXN) goto end;
                }
            }
        }
end:;
    }
    return field;
}
