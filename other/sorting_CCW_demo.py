import pygame
import time
import math

pygame.init()

def get_max(list):
    max = list[0]
    for item in list:
        if item[1] < max[1]:
            max = item
    return max

def get_min(list):
    min = list[0]
    for item in list:
        if item[1] > min[1]:
            min = item
    return min
    

def sort(list):
    max = get_max(list)
    min = get_min(list)
    original_length = len(list)

    new_list = []
    new_list.append(list.pop(list.index(max)))

    high = True
    while len(new_list) < original_length:
        max_score = "X"
        max_item = "X"
        current_item = new_list[-1]
        print(current_item)
        print(min)

        if current_item == min:
            print("MIN")
            high = False
            max = get_max(list)
        elif current_item == max:
            print("MAX")
            high = True
            min = get_min(list)

        print(f"Pass {len(new_list)}")
        print(list)
        print(new_list)
        for item in list:
            if ( item[1] == current_item[1] ):
                dX = current_item[0]
                if max_score == "X" or abs(dX) > max_score:
                    max_score = abs(dX)
                    max_item = item

            elif ( (high and item[1] > current_item[1]) or (not high and item[1] < current_item[1]) ):
                dX = current_item[0] - item[0]
                dY = current_item[1] - item[1]
       
                if max_score == "X" or dX/-dY > max_score:
                    max_score = dX/-dY
                    max_item = item

        new_list.append(list.pop(list.index(max_item)))
    print(f"Final Result: {len(new_list)}")
    print(list)
    print(new_list)
    return new_list


class App:
    def __init__(self):
        self.screen = pygame.display.set_mode((640, 320))
        self.caption = pygame.display.set_caption("CCW Sorting Demo")
        self.running = True

    def run(self):
        done_flag = False
        coordinate_list = []
        while self.running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    if not done_flag:
                        if event.button == 1:
                            mouse_pos = pygame.mouse.get_pos()
                            self.screen.set_at(pygame.mouse.get_pos(), "#00FF00")
                            coordinate_list.append(mouse_pos)

                        elif event.button == 3:
                            sorted_list = sort(coordinate_list)
                            self.draw_lines(sorted_list)




            pygame.display.flip()

    def draw_lines(self, list):
        for i in range(len(list) - 1):
            pygame.draw.line(self.screen, "#00FF00", list[i], list[i + 1])
            # for j in range(0, i):
            #     pygame.draw.line(self.screen, "#00FF00", list[j], list[j + 1])
            # time.sleep(0.5)
        


if __name__ == "__main__":
    app = App()
    app.run()
        
