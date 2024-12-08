#include <stdio.h>

int m1[25][25] ;

int main(){
    int n, m ;
    scanf("%d %d", &n, &m) ;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            scanf("%d", &m1[i][j]) ;
        }
    }
    printf("%d %d\n", n, m) ;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            int sum = 0 ;
            for(int di = -1; di <=1; di++){
                for(int dj = -1; dj<=1; dj++){
                    if(di == 0 && dj == 0) continue ;
                    if(i+di>=0 && i+di<n && j+dj>=0 && j+dj<m)
                        sum += m1[i+di][j+dj] ;
                }
            }
            if(m1[i][j]){
                if(sum>=2 && sum<=3){
                    printf("1") ;
                } else printf("0") ;
            } else {
                if(sum == 3){
                    printf("1") ;
                } else printf("0") ;
            }
            if(j<m-1) printf(" ") ;
        }
        printf("\n") ;
    }
    return 0 ;
}
