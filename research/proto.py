#!/usr/bin/env python3

class   Strategy:
    def __init__(self):
        self.child = []
    def input(self):
        pass

class   ViewStrategy(Strategy):
    def input(self, objs):
        res = 1
        for ob in objs:
            res = res * ob
        for children in self.child:
            children.input(res)

class   IntelStrategy(Strategy):
    def input(self, val):
        if (val > 5):
            for children in self.child:
                children.input()
            
class   ActionStrategy(Strategy):
    def input(self):
        print("Move forward\n")



## Construction of the decision tree ##
entry = ViewStrategy()

intel = IntelStrategy()

entry.child.append(intel)
intel.child.append(ActionStrategy())

import random
import time

r = random.Random()

while 1:
    entry.input([42, -5, 3, -1, r.random() * 0.1, r.random()])
    print("_" * 15)
    time.sleep(0.3)
