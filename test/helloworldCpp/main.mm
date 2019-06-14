//
//  main.m
//  helloworldCpp
//
//  Created by User on 2019/6/13.
//  Copyright © 2019 User. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Window.hpp"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // insert code here...
        Window window(800, 600, (GLchar *)"Hello world c plus plus");
        window.render();
    }
    return 0;
}
