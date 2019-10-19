//
//  main.c
//  CShell_CISC361_Project2
//
//  Created by MiguelZN on 10/4/19.
//  Copyright © 2019 MiguelZN. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include "get_path.h"

int main(int argc, char **argv, char **envp ){
    sh(argc,argv,envp);

    return 0;
}
