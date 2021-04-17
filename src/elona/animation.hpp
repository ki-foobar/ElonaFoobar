#pragma once

#include "color.hpp"
#include "optional.hpp"
#include "position.hpp"



namespace elona
{

struct Character;



class AbstractAnimation
{
public:
    void play();


protected:
    virtual void do_play() = 0;


    virtual bool updates_screen()
    {
        return true;
    }
};



class FailureToCastAnimation : public AbstractAnimation
{
public:
    FailureToCastAnimation(const Position& caster_pos)
        : caster_pos(caster_pos)
    {
    }


protected:
    virtual void do_play() override;


private:
    const Position& caster_pos;
};



class BrightAuraAnimation : public AbstractAnimation
{
public:
    enum class Type
    {
        debuff,
        offering,
        healing,
        healing_rain,
    };


    BrightAuraAnimation(const Position& target_pos, Type type)
        : target_pos(target_pos)
        , type(type)
    {
    }


protected:
    virtual void do_play() override;


private:
    const Position& target_pos;
    Type type;
};



class BreathAnimation : public AbstractAnimation
{
public:
    BreathAnimation(
        const Position& attacker_pos,
        const Position& target_pos,
        const std::vector<Position>& breath_route,
        int element)
        : attacker_pos(attacker_pos)
        , target_pos(target_pos)
        , breath_route(breath_route)
        , element(element)
    {
    }


protected:
    virtual void do_play() override;


private:
    const Position& attacker_pos;
    const Position& target_pos;
    const std::vector<Position>& breath_route;
    int element;
};



class BallAnimation : public AbstractAnimation
{
public:
    enum class Type
    {
        ball,
        atomic_bomb,
    };


    BallAnimation(
        const Position& position,
        int range,
        Type type,
        int element = 0)
        : position(position)
        , range(range)
        , type(type)
        , element(element)
    {
    }


protected:
    virtual void do_play() override;


private:
    const Position& position;
    int range;
    Type type;
    int element;
};



class BoltAnimation : public AbstractAnimation
{
public:
    BoltAnimation(
        const Position& attacker_pos,
        const Position& target_pos,
        int element,
        int distance,
        const std::vector<std::pair<int, int>>& route)
        : attacker_pos(attacker_pos)
        , target_pos(target_pos)
        , element(element)
        , distance(distance)
        , route(route)
    {
    }


protected:
    virtual void do_play() override;


private:
    const Position& attacker_pos;
    const Position& target_pos;
    int element;
    int distance;
    const std::vector<std::pair<int, int>>& route;
};



class ThrowingObjectAnimation : public AbstractAnimation
{
public:
    ThrowingObjectAnimation(
        const Position& thrower_pos,
        const Position& target_pos,
        int item_chip,
        const Color& item_color = {255, 255, 255})
        : thrower_pos(thrower_pos)
        , target_pos(target_pos)
        , item_chip(item_chip)
        , item_color(item_color)
    {
    }


protected:
    virtual void do_play() override;


private:
    const Position& thrower_pos;
    const Position& target_pos;
    int item_chip;
    const Color& item_color;
};



class SwarmAnimation : public AbstractAnimation
{
public:
    SwarmAnimation(const Position& target_pos)
        : target_pos(target_pos)
    {
    }


protected:
    virtual void do_play() override;


private:
    const Position& target_pos;
};



class RangedAttackAnimation : public AbstractAnimation
{
public:
    enum class Type
    {
        magic_arrow = 0,
        distant_attack = 1,
        bow = 108,
        crossbow = 109,
        firearm = 110,
        throwing = 111,
    };


    RangedAttackAnimation(
        const Position& attacker_pos,
        const Position& target_pos,
        Type type,
        int fired_item_subcategory = 0,
        int fired_item_image = 0,
        const Color& fired_item_color = {255, 255, 255})
        : attacker_pos(attacker_pos)
        , target_pos(target_pos)
        , type(type)
        , fired_item_subcategory(fired_item_subcategory)
        , fired_item_image(fired_item_image)
        , fired_item_color(fired_item_color)
    {
    }


protected:
    virtual void do_play() override;


private:
    const Position& attacker_pos;
    const Position& target_pos;
    Type type;
    int fired_item_subcategory;
    int fired_item_image;
    const Color& fired_item_color;
};



class MeleeAttackAnimation : public AbstractAnimation
{
public:
    MeleeAttackAnimation(
        const Position& position,
        bool debris,
        int attack_skill,
        int damage_percent,
        bool is_critical)
        : position(position)
        , debris(debris)
        , attack_skill(attack_skill)
        , damage_percent(damage_percent)
        , is_critical(is_critical)
    {
    }


protected:
    virtual void do_play() override;

private:
    const Position& position;
    bool debris;
    int attack_skill;
    int damage_percent;
    bool is_critical;
};



class GeneEngineeringAnimation : public AbstractAnimation
{
public:
    GeneEngineeringAnimation(const Position& position)
        : position(position)
    {
    }


protected:
    virtual void do_play() override;

private:
    const Position& position;
};



class MiracleAnimation : public AbstractAnimation
{
public:
    enum class Mode
    {
        target_one,
        target_all,
    };


    MiracleAnimation(Mode mode, const Character& chara)
        : _mode(mode)
        , _chara(chara)
    {
    }


protected:
    virtual void do_play() override;

private:
    Mode _mode;
    const Character& _chara;
};



class MeteorAnimation : public AbstractAnimation
{
public:
    MeteorAnimation()
    {
    }


protected:
    virtual void do_play() override;
};



class RagnarokAnimation : public AbstractAnimation
{
public:
    RagnarokAnimation()
    {
    }


protected:
    virtual void do_play() override;


    virtual bool updates_screen() override
    {
        return false;
    }
};



class BreakingAnimation : public AbstractAnimation
{
public:
    BreakingAnimation(const Position& position)
        : position(position)
    {
    }


protected:
    virtual void do_play() override;


private:
    Position position;
};



void animeblood(const Character& chara, int animation_type, int element);
void animeload(int animation_type, const Character& chara);

} // namespace elona
