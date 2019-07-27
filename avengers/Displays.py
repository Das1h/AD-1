import pygame
from pygame.locals import *
import sys

cs = 'cs'
bi = 'bi'
ia = 'ia'
class Button:
    def __init__(self, Type, point):
        self.img = pygame.image.load("./img/" + Type + "Button.png")
        self.rect = self.img.get_rect(topleft = point)
        self.buttonType = Type
        self.isPressed = False
    
    def press(self):
        if self.isPressed == False:
            self.img = pygame.image.load("./img/pressed_" + self.buttonType + "Button.png")
            self.isPressed = True
        else: 
            self.img = pygame.image.load("./img/" + self.buttonType + "Button.png")
            self.isPressed = False


csButton = Button(cs, (50,100))
biButton = Button(bi, (150,100))
iaButton = Button(ia, (250,100))
csButton.press()        #csを初期状態にしとく
nowPressed = 'cs'


def displayAllButtons(screen):
    screen.blit(csButton.img, csButton.rect)
    screen.blit(biButton.img, biButton.rect)
    screen.blit(iaButton.img, iaButton.rect)