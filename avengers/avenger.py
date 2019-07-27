import pygame
from pygame.locals import *
import sys

import chara
import Displays

def main():
    (width, height) = (1920, 1080)   # 画面サイズ
    pygame.init()       # pygame初期化
    pygame.display.set_mode((width, height), 0)  # 画面設定
    screen = pygame.display.get_surface()
    
    # 背景の取得
    backGround = pygame.image.load("./img/bg.png").convert()
    rect_backGround = backGround.get_rect()

    #FPS設定用のなんか
    clock = pygame.time.Clock()
     

    while (1):
        clock.tick(60)      #FPS
        pygame.display.update()             # 画面更新
        screen.fill((0, 0, 0))          # 画面の背景色
        screen.blit(backGround, rect_backGround)            # 背景画像の描画

        #レーン表示
        lineColor = (255, 255, 255)
        pygame.draw.polygon(screen, lineColor, [[380,550], [1930,550], [1930,1045], [135,1045]], 3)
        for i in range(1, 5):
            lineX = 380 - 49 * i
            lineY = 550 + 99 * i
            pygame.draw.polygon(screen, lineColor, [[lineX, lineY], [1930, lineY]], 3)

        #キャラ表示
        chara.spriteGroup.update(screen)

        #ボタン表示
        Displays.displayAllButtons(screen)

        #イベント処理
        for event in pygame.event.get():
            # 閉じるボタンが押されたとき
            if event.type == QUIT:
                pygame.quit()
                sys.exit()

            # キーを押したとき
            if event.type == KEYDOWN:
                if event.key == K_ESCAPE:   # Escキーが押されたとき
                    pygame.quit()
                    sys.exit()
                if event.key == K_e:
                    chara.MakeEnemy().changeStat(chara.alive)
                if event.key == K_r:
                    pass

            #クリックイベント
            if event.type == MOUSEBUTTONDOWN:
                mousePoint = pygame.mouse.get_pos()
                (mouseX, mouseY) = mousePoint
                print(mousePoint)
                if 550 <= mouseY and mouseY < 649:
                    print('lane0')
                    chara.MakeFriend(0).changeStat(chara.alive)
                elif 649 <= mouseY and mouseY < 748:
                    print('lane1')
                    chara.MakeFriend(1).changeStat(chara.alive)
                elif 748 <= mouseY and mouseY < 847:
                    print('lane2')
                    chara.MakeFriend(2).changeStat(chara.alive)
                elif 847 <= mouseY and mouseY < 946:
                    print('lane3')
                    chara.MakeFriend(3).changeStat(chara.alive)
                elif 946 <= mouseY and mouseY < 1045:
                    print('lane4')
                    chara.MakeFriend(4).changeStat(chara.alive)

                if Displays.csButton.rect.collidepoint(mousePoint):
                    if Displays.nowPressed == 'cs':
                        pass
                    elif Displays.nowPressed == 'ia':
                        Displays.csButton.press()
                        Displays.iaButton.press()
                        Displays.nowPressed = 'cs'
                    elif Displays.nowPressed == 'bi':
                        Displays.csButton.press()
                        Displays.biButton.press()
                        Displays.nowPressed = 'cs'

                elif Displays.biButton.rect.collidepoint(mousePoint):
                    if Displays.nowPressed == 'bi':
                        pass
                    elif Displays.nowPressed == 'ia':
                        Displays.biButton.press()
                        Displays.iaButton.press()
                        Displays.nowPressed = 'bi'
                    elif Displays.nowPressed == 'cs':
                        Displays.biButton.press()
                        Displays.csButton.press()
                        Displays.nowPressed = 'bi'

                elif Displays.iaButton.rect.collidepoint(mousePoint):
                    if Displays.nowPressed == 'ia':
                        pass
                    elif Displays.nowPressed == 'cs':
                        Displays.iaButton.press()
                        Displays.csButton.press()
                        Displays.nowPressed = 'ia'
                    elif Displays.nowPressed == 'bi':
                        Displays.iaButton.press()
                        Displays.biButton.press()
                        Displays.nowPressed = 'ia'


if __name__ == "__main__":
    main()