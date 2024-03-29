import pygame
from pygame.locals import *
import sys
import random

import Displays

#じょうたい
notExist = 'notExist'
alive = 'alive'
battle = 'battle'

#chara type
cs = 'cs'
bi = 'bi'
ia = 'ia'
ako3 = 'ako3'
fkt3 = 'fkt3'
som3 = 'som3'
#キャラ画像一蘭
imgList = {cs:'./chara/cs.png', bi:'./chara/bi.png', ia:'./chara/ia.png', ako3:'./chara/ako3.png', fkt3:'./chara/fkt3.png', som3:'./chara/som3.png'}
#たたかえるげんかい
attackLimit = 5

class Chara(pygame.sprite.Sprite):
    def __init__(self, hitpoint, attack, speed, range, chara_type): 
        super().__init__()
        self.HP = hitpoint
        self.ATK = attack
        self.SPD = speed
        self.RNG = range
        self.charaType = chara_type
        self.img = pygame.image.load(imgList[chara_type]).convert_alpha()
        self.stat = notExist
        self.battleTime = 0   #戦ったじかん
        self.friendly = True   #初期値は味方判定
        self.attackDelay = int(100/speed)   #攻撃にかかる時間
        self.attackCount = 0   #攻撃した回数

    #画像表示するぜ
    def draw(self, screen):
        screen.blit(self.img, self.rect)

    #Spriteの更新 1フレーム毎に行うぜ
    def update(self, screen):
        if self.stat == notExist:
            self.kill()
        elif self.stat == alive:
            self.draw(screen)
            if self.friendly == True:
                self.rect.move_ip(self.SPD, 0)
                self.rangeRect.move_ip(self.SPD, 0)
            else:
                self.rect.move_ip(-self.SPD, 0)
                self.rangeRect.move_ip(-self.SPD, 0)
        elif self.stat == battle:
            self.draw(screen)
            self.battleTime += 1
            if attackLimit < self.attackCount:      #戦闘限界
                self.changeStat(notExist)
            
        #画面外行ったら時
        if self.rect.left >= 1920 or self.rect.right <= 0:
            self.kill()

        #experiment
        pygame.draw.rect(screen, (255, 0, 0), self.rangeRect, 4)

    #キャラの状態を変えるぜ
    def changeStat(self, newStat):
        self.stat = newStat
        print(self.charaType, newStat)
        if newStat == battle:
            self.loadImg('./chara/' + self.charaType + '_battle.png')
        elif newStat == alive:
            self.loadImg(imgList[self.charaType])
        elif newStat == notExist:
            self.kill()

    #きゃらのがぞう読み込む
    def loadImg(self, imgAddress):  #imgAddress = String
        self.img = pygame.image.load(imgAddress).convert_alpha()

    #攻撃するぜ！
    def Attack(self, enemy):
        if self.battleTime >= self.attackDelay:
            enemy.HP -= self.ATK
            self.attackCount += 1
            print(self.charaType, 'attacked!!!!!!!')
            if enemy.HP <= 0:
                enemy.changeStat(notExist)
                self.changeStat(alive)
            self.battleTime = 0

#みかた
class Friends(Chara):
    def __init__(self, hitpoint, attack, speed, range, chara_type, laneNum, cost):
        super().__init__(hitpoint, attack, speed, range, chara_type)
        self.rect = Rect(self.img.get_rect(center = friend_pos[laneNum]))
        self.Cost = cost
        self.friendly = True
        self.rangeRect = Rect(self.rect.center, (range, 5))

    def update(self, screen):
        super().update(screen)
        #戦闘状態へ移行
        enemy_list = enemyGroup.sprites()
        for enemy in enemy_list:
            if self.rangeRect.colliderect(enemy.rect) == True and self.rect.centery == enemy.rect.centery:
                if self.stat != battle:
                    self.changeStat(battle)
                else:
                    self.Attack(enemy)
            elif self.stat == battle:
                self.changeStat(alive)
        #戦闘状態解除
        if enemyGroup.bool == False:    #敵居なかったら戦闘状態解除しちゃうぜ
            self.stat = alive


#てき
class Enemy(Chara):
    def __init__(self, hitpoint, attack, speed, range, chara_type):
        super().__init__(hitpoint, attack, speed, range, chara_type)
        self.rect = Rect(self.img.get_rect(center = enemy_pos[random.randint(0,4)]))
        self.friendly = False
        (x, y) = self.rect.center
        self.rangeRect = Rect(x - range, y, range, 5)

    def update(self, screen): 
        super().update(screen)
        #戦闘状態へ移行
        friend_list = friendGroup.sprites()
        for friend in friend_list:
            if self.rangeRect.colliderect(friend.rect) == True and self.rect.centery == friend.rect.centery:
                if self.stat != battle:
                    self.changeStat(battle)
                else:
                    self.Attack(friend)
            elif self.stat == battle:
                print(self.charaType, '敵いないやんけ')
                self.changeStat(alive)


#スプライトのグループだよ
spriteGroup = pygame.sprite.Group() #ぜんぶ
friendGroup = pygame.sprite.Group()
enemyGroup = pygame.sprite.Group()

#キャラ管理するリスト
enemy_list = enemyGroup.sprites()
friend_list = friendGroup.sprites()

#キャラ召喚位置の座標
friend_pos = [(380,550), (331,649), (281,748), (233,847), (184,946)]
enemy_pos = [(1870,550), (1870,649), (1870,748), (1870,847), (1870,946)]



#chara---(HP, ATK, SPD, range, type, lane, cost)
#生徒つくる
def MakeFriend(laneNumber):
    if Displays.nowPressed == cs:
        newFriend = Friends(4, 4, 1, 800, cs, laneNumber, 3)
    elif Displays.nowPressed == bi:
        newFriend = Friends(10, 3, 4, 50, bi, laneNumber, 1)
    elif Displays.nowPressed == ia:
        newFriend = Friends(18, 1, 2, 85, ia, laneNumber, 2)
    spriteGroup.add(newFriend)
    friendGroup.add(newFriend)
    return newFriend

#てきつくる
def MakeEnemy():
    r = random.random() #敵出現の確率用
    if r <= 0.5:       #som出現確率(50%)
        newEnemy = Enemy(10, 3, 4, 50, som3)
    if 0.5 < r <= 0.8: #ako3出現確率(30%)
        newEnemy = Enemy(18, 1, 2, 85, ako3)
    if r > 0.8:         #fkt3出現確率(20%)
        newEnemy = Enemy(4, 4, 1, 800, fkt3)
    spriteGroup.add(newEnemy)
    enemyGroup.add(newEnemy)
    print('enemy')
    return newEnemy