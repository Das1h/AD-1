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
cs_battle = 'cs_battle'
bi = 'bi'
bi_battle = 'bi_battle'
ia = 'ia'
ia_battle = 'ia_battle'
ako3 = 'ako3'
ako3_battle = 'ako3_battle'
fkt3 = 'fkt3'
fkt3_battle = 'fkt3_battle'
som3 = 'som3'
som3_battle = 'som3_battle'



#たたかえるげんかい
attackLimit = 5

class Chara(pygame.sprite.Sprite):
    def __init__(self, hitpoint, attack, speed, range, chara_type, imageList):
        super().__init__()
        self.HP = hitpoint
        self.ATK = attack
        self.SPD = speed
        self.RNG = range
        self.charaType = chara_type
        self.img = imageList[chara_type]
        self.stat = notExist
        self.battleTime = 0   #戦ったじかん
        self.friendly = True   #初期値は味方判定
        self.attackDelay = int(100/speed)   #攻撃にかかる時間
        self.attackCount = 0   #攻撃した回数
        self.imgList = imageList

    #画像表示するぜ
    def draw(self, screen):
        screen.blit(self.img, self.rect)

    #Spriteの更新 1フレーム毎に行うぜ
    def update(self, screen):
        if self.stat == notExist:
            self.kill()
        elif self.stat == alive:
            self.draw(screen)
            #移動
            if self.friendly == True:
                self.rect.move_ip(self.SPD, 0)
                self.rangeRect.move_ip(self.SPD, 0)
            else:
                self.rect.move_ip(-self.SPD, 0)
                self.rangeRect.move_ip(-self.SPD, 0)
        elif self.stat == battle:
            self.draw(screen)
            self.battleTime += 1
            #戦闘限界
            if attackLimit < self.attackCount:
                self.changeStat(notExist)
        #画面外行ったら時
        if self.rect.left >= 1920 or self.rect.right <= 0:
            self.kill()

        #experiment
        #pygame.draw.rect(screen, (255, 0, 0), self.rangeRect, 4)

    #キャラの状態を変えるぜ
    def changeStat(self, newStat):
        self.stat = newStat
        if newStat == battle:
            self.img = self.imgList[self.charaType + '_battle']
        elif newStat == alive:
            self.img = self.imgList[self.charaType]
        elif newStat == notExist:
            self.kill()

    #攻撃するぜ！
    def Attack(self, enemy):
        if self.battleTime >= self.attackDelay:
            enemy.HP -= self.ATK
            self.attackCount += 1
            print(self.charaType, 'attacked!!!!!!!')
            if enemy.HP <= 0:
                enemy.changeStat(notExist)
            self.battleTime = 0


#みかた
class Friends(Chara):
    def __init__(self, hitpoint, attack, speed, range, chara_type, imageList, laneNum, cost):
        super().__init__(hitpoint, attack, speed, range, chara_type, imageList)
        self.rect = Rect(self.img.get_rect(center = friend_pos[laneNum]))
        self.Cost = cost
        self.friendly = True
        self.rangeRect = Rect(self.rect.center, (range, 5))

    def update(self, screen): 
        super().update(screen)
        #戦闘状態へ移行
        battleFlag = False
        for enemy in enemyGroup:
            #味方→敵への攻撃系統
            if self.rangeRect.colliderect(enemy.rect) == True and self.rect.centery == enemy.rect.centery:
                battleFlag = True
                if self.stat != battle:
                    self.changeStat(battle)
                else:
                    self.Attack(enemy)
        if battleFlag == False:
            self.changeStat(alive)


#てき
class Enemy(Chara):
    def __init__(self, hitpoint, attack, speed, range, chara_type, imageList):
        super().__init__(hitpoint, attack, speed, range, chara_type, imageList)
        self.rect = Rect(self.img.get_rect(center = enemy_pos[random.randint(0,4)]))
        self.friendly = False
        (x, y) = self.rect.center
        self.rangeRect = Rect(x - range, y, range, 5)

    def update(self, screen): 
        super().update(screen)
        #戦闘状態へ移行
        battleFlag = False
        for friend in friendGroup:
            #味方→敵への攻撃系統
            if self.rangeRect.colliderect(friend.rect) == True and self.rect.centery == friend.rect.centery:
                battleFlag = True
                if self.stat != battle:
                    self.changeStat(battle)
                else:
                    self.Attack(friend)
        if battleFlag == False:
            for baseFront in friend_pos:
                (x, y) = baseFront
                if self.rect.centerx <= x and self.rect.centery == y:
                    self.changeStat(battle)
                    self.attackBase(MyBase)
                    break
                else:
                    self.changeStat(alive)

    def attackBase(self, base):
        if self.battleTime >= self.attackDelay:
            base.hitpoint -= self.ATK
            self.attackCount += 1
            self.battleTime = 0
            print(self.charaType, 'attacked base!!!!!!')
            if base.hitpoint <= 0:
                pygame.quit()
                sys.exit()


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
def MakeFriend(laneNumber, imageList):
    if Displays.nowPressed == cs:
        newFriend = Friends(4, 4, 1, 800, cs, imageList, laneNumber, 3)
    elif Displays.nowPressed == bi:
        newFriend = Friends(10, 3, 4, 50, bi, imageList, laneNumber, 1)
    elif Displays.nowPressed == ia:
        newFriend = Friends(18, 1, 2, 85, ia, imageList, laneNumber, 2)
    spriteGroup.add(newFriend)
    friendGroup.add(newFriend)
    return newFriend

#てきつくる
def MakeEnemy(imageList):
    r = random.random() #敵出現の確率用
    if r <= 0.5:       #som出現確率(50%)
        newEnemy = Enemy(10, 3, 4, 50, som3, imageList)
    elif 0.5 < r <= 0.8: #ako3出現確率(30%)
        newEnemy = Enemy(18, 1, 2, 85, ako3, imageList)
    elif r > 0.8:         #fkt3出現確率(20%)
        newEnemy = Enemy(4, 4, 1, 800, fkt3, imageList)
    spriteGroup.add(newEnemy)
    enemyGroup.add(newEnemy)
    return newEnemy

class Base():
    def __init__(self):
        self.hitpoint = 100

MyBase = Base()