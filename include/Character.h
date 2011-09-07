#ifndef CHARACTER_H
#define CHARACTER_H

#include <algorithm>
#include <deque>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

typedef float Stat;
typedef int Attribute;

Stat add(Stat a1, Stat a2) {
    return a1+a2;
}

Stat subtract(Stat a1, Stat a2) {
    return a1-a2;
}

Stat multiply(Stat a1, Stat a2) {
    return a1*a2;
}

Stat divide(Stat a1, Stat a2) {
    return a1/a2;
}

union Attributes
{
    struct
    {

        Attribute str,con,tgh,dex,rsn,lck;
    };
    Attribute value[6];
};


struct Stats
{
private:
    Stats calc(const Stats& stat, Stat (&f)(Stat,const Stat)) {
        for(int i=0;i<count;++i)
            values[i] = f(values[i],stat.values[i]);
        return *this;
    }
public:
    Stats() : values(count) {}
    enum { mindmg=0,maxdmg,hit,dodge,crithit,critdmg,critdodge,minarm,maxarm,counterstrike,hp,ap,attackspeed,count};
    vector<Stat> values;

    Stats operator +=(const Stats& stat) {
        return calc(stat,add);
    }

    Stats operator +(const Stats& stat) {
        Stats newStats = *this;
        return (newStats+=stat);
    }

    Stats operator -=(const Stats& stat) {
        return calc(stat,subtract);
    }

    Stats operator -(const Stats& stat) {
        Stats newStats = *this;
        return (newStats-=stat);
    }

    Stats operator *=(const Stats& stat) {
        return calc(stat,multiply);
    }

    Stats operator *(const Stats& stat) {
        Stats newStats = *this;
        return (newStats*=stat);
    }

    Stats operator /=(const Stats& stat) {
        return calc(stat,divide);
    }

    Stats operator /(const Stats& stat) {
        Stats newStats = *this;
        return (newStats/=stat);
    }
};

class Calculator
{
public:
    Stats calcStats(const Attributes& attr) {
        Stats stat;
        stat.values[Stats::mindmg] = attr.str*0.6;
        stat.values[Stats::maxdmg] = attr.str*1.5;
        stat.values[Stats::hit] = attr.dex;
        stat.values[Stats::dodge] = attr.con;
        stat.values[Stats::crithit] = (attr.lck+attr.dex) / 5.0;
        stat.values[Stats::critdmg] = (attr.lck+attr.dex) / 8.0;
        stat.values[Stats::critdodge] = (attr.lck+attr.con) / 8.0;
        stat.values[Stats::minarm] = attr.tgh*0.4;
        stat.values[Stats::maxarm] = attr.tgh*0.8;
        stat.values[Stats::counterstrike] = (attr.con+2*attr.rsn)/6.5;
        stat.values[Stats::hp] = attr.tgh*8+60;
        stat.values[Stats::ap] = (attr.tgh+attr.rsn)*5;
        stat.values[Stats::attackspeed] = 10;
        return stat;
    }
    float sigmoid(float value) {
        return 1.0 / (1.0 + exp(-value/5.2));
    }
    float random() {
        return (rand()%1000)/10.0;
    }
    float random(float min,float max) {
        float p = random()/100.0;
        return p*min + (1 - p)*max;
    }
};

struct Buff
{
    Stats stats;
    int alive;
};

struct Equip
{
    enum Type {lhand = 0,rhand, torso,legs,helm,foot,arms,cape,twohanded,count};
    Type type;
    Stats stats;
};

class Char
{
public:
    Char() : statsChanged(true), buffs(0), equipment(Equip::count,0)
    {
        attributes.str = 0;
        attributes.con = 0;
        attributes.tgh = 0;
        attributes.dex = 0;
        attributes.rsn = 0;
        attributes.lck = 0;
    }
    typedef deque<Buff> Buffs;
    Stats basicStats,stats;
    int attackCooldown,currentHP;
    vector<const Equip*> equipment;
    bool statsChanged;
    void addEquip(const Equip* eqp) {
        if(eqp->type == Equip::twohanded) {
            unequip(Equip::lhand);
            unequip(Equip::rhand);
        } else if(eqp->type == Equip::lhand || eqp->type == Equip::rhand) {
            unequip(Equip::twohanded);
        }
        equip(eqp);
    }
    void addBuff(const Buff& buff) {
        buffs.push_back(buff);
        statsChanged = true;
    }
    void updateBuffs() {
        for(Buffs::iterator buff = buffs.begin(); buff != buffs.end();) {
            if(buff->alive>0) buff->alive--;
            if(buff->alive == 0) {
                *buff = *buffs.rbegin();
                buffs.pop_back();
                statsChanged = true;
            }
            else {
                buff++;
            }
        }
    }
    Stats getStats() {
        if(statsChanged) recalcStats();
        return stats;
    }

    void recalcStats() {
        stats = basicStats;
        for(Buffs::const_iterator buff = buffs.begin(); buff != buffs.end(); ++buff) {
            stats += buff->stats;
        }
        for(int i=0;i<Equip::count;i++) {
            if(equipment[i]) {
                stats += equipment[i]->stats;
            }
        }
        statsChanged = false;
    }

    Buffs buffs;
    Attributes attributes;
private:
    void equip(const Equip* eqp) {
        if(equipment[eqp->type]) unequip(eqp->type);
        equipment[eqp->type] = eqp;
        statsChanged = true;
    }
    void unequip(Equip::Type type) {
        if(equipment[type]) {
            equipment[type] = 0;
            statsChanged = true;
        }
    }
};

void attack(Char& atk, Char& def, Calculator& calc) {
    int state = 0;
    if(calc.random() < 100.0*calc.sigmoid(atk.getStats().values[Stats::hit] - def.getStats().values[Stats::dodge])) {
        state = 2;
    }
    if(state == 0 &&
       calc.random() < atk.getStats().values[Stats::crithit]) {
        state = 3;
    }
    if(state != 0 &&
       calc.random() < def.getStats().values[Stats::critdodge]) {
        state = 1;
    }
    if(calc.random() < def.getStats().values[Stats::counterstrike]) {
        attack(def,atk,calc);
    }
    if(state > 1) {
        float damage = calc.random(atk.getStats().values[Stats::mindmg],
                                 atk.getStats().values[Stats::maxdmg]);
        if(calc.random() < atk.getStats().values[Stats::critdmg]) {
            state = 4;
        } else {
            damage -= calc.random(def.getStats().values[Stats::minarm],
                                  def.getStats().values[Stats::maxarm]);
        }
        if(damage > 0)
            def.currentHP -= damage;
    }
}

#endif
