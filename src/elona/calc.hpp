#pragma once

#include <string>

#include "enums.hpp"
#include "eobject/forward.hpp"
#include "optional.hpp"
#include "typedefs.hpp"



namespace elona
{

struct Character;
struct Item;



struct SkillDamage
{
    lua_int dice_x;
    lua_int dice_y;
    lua_int damage_bonus;
    int element;
    int element_power;
};

optional<SkillDamage>
calc_skill_damage(const Character& chara, int skill, int power);


int calcobjlv(int = 0);
Quality calcfixlv(Quality base_quality = Quality::none);
int calc_gained_fame(const Character& chara, int base);
int decrease_fame(Character& chara, int base);
int calcshopreform();
int calc_rate_to_pierce(int);
std::string calc_age(const Character& chara);
int calcexpalive(int = 0);
int calc_evasion(const Character& chara);
int calc_accuracy(
    const Character& attacker,
    const Character& target,
    const OptionalItemRef& weapon,
    const OptionalItemRef& ammo,
    bool consider_distance);
int calcattackhit(
    const Character& attacker,
    const Character& target,
    const OptionalItemRef& weapon,
    const OptionalItemRef& ammo);


enum class AttackDamageCalculationMode
{
    actual_damage,
    raw_damage,
};
int calcattackdmg(
    const Character& attacker,
    const Character& target,
    const OptionalItemRef& weapon,
    const OptionalItemRef& ammo,
    AttackDamageCalculationMode);


struct CalcAttackProtectionResult
{
    lua_int rate;
    lua_int dice_x;
    lua_int dice_y;
};
CalcAttackProtectionResult calc_attack_protection(const Character& chara);

int calcmedalvalue(const ItemRef& item);
int calcitemvalue(const ItemRef& item, int calc_mode);
int calcinvestvalue(const Character& shopkeeper);
int calcguiltvalue();
int calc_adventurer_hire_cost(const Character& adv);
int calc_servant_hire_cost(const Character& servant);
void generatemoney(Character& chara);
void calccosthire();
int calccostbuilding();
int calccosttax();
int calcmealvalue();
int calc_ammo_reloading_cost(Character& owner, bool do_reload = false);
int calccargoupdate();
int calccargoupdatecost();
int calcidentifyvalue(int);
int calc_skill_training_cost(
    int skill_id,
    const Character& chara,
    bool discount = false);
int calc_skill_learning_cost(
    int skill_id,
    const Character& chara,
    bool discount = false);
int calc_resurrection_value(const Character& chara);
int calc_slave_value(const Character& chara);
int calcrestorecost();
int calcinitgold(int);
int calc_spell_power(const Character& caster, int id);
int calc_spell_success_rate(const Character& caster, int id);
int calc_spell_cost_mp(const Character& caster, int id);
int calc_spell_cost_stock(const Character& caster, int id);
lua_int calcscore();
void calcpartyscore();
void calcpartyscore2();

int generate_color(ColorIndex index, int id);

/**
 * Calculates the base potential of a skill being initialized, without taking
 * into account potential decay based on the character's level.
 *
 * @param skill_id The skill id
 * @param original_level The intrinsic skill level of the character
 * @param initial_level The initial skill level coefficient
 *
 * @return The base potential
 */
int calc_initial_skill_base_potential(
    int skill_id,
    int original_level,
    int initial_level);

/**
 * Calculates the new potential when a skill level is gained.
 *
 * @param potential The potential before leveling
 *
 * @return The potential after leveling
 */
int calc_potential_on_gain(int potential);

/**
 * Calculates the new potential when a skill level is lost.
 *
 * @param potential The potential before leveling
 *
 * @return The potential after leveling
 */
int calc_potential_on_loss(int potential);

/**
 * Calculates the initial speed level of a character.
 *
 * @param initial_level The initial speed level coefficient
 * @param chara_level The character's level
 *
 * @return The adjusted speed level
 */
int calc_initial_skill_level_speed(int initial_level, int chara_level);

/**
 * Calculates the initial level of a skill. Used for all skills/spells
 * except speed.
 *
 * @param initial_level The initial skill level coefficient
 * @param chara_level The character's level
 * @param potential The character's potential in the skill
 *
 * @return The adjusted skill level
 */
int calc_initial_skill_level(int initial_level, int chara_level, int potential);

/**
 * Calculates the decayed potential of a skill based on a character's level. It
 * will be unchanged if the character's level is 1.
 *
 * @param chara_level The character's level
 * @param potential The initial potential amount
 *
 * @return The adjusted potential amount
 */
int calc_initial_skill_decayed_potential(int chara_level, int potential);

/**
 * Calculates a character's initial resistance level for an element.
 * It is based on the character's level.
 *
 * - If the character is the player, it is 100.
 * - If the element is "magic" and the calculated level is below 500,
     it is 100.
 *
 * @param chara The character
 * @param initial_level The intrinsic level of the resistance
 * @param element_id The element type of the resistance
 *
 * @return The adjusted resistance level
 */
int calc_initial_resistance_level(
    const Character& chara,
    int initial_level,
    int element_id);

/**
 * Calculates the amount of experience gained by an attribute related
 * to a skill which gained experience.
 *
 * @param experience The amount of experience gained by the related skill
 * @param divisor The attribute's experience divisor for related skills
 *
 * @return The adjusted experience amount
 */
int calc_skill_related_attribute_exp(int experience, int divisor);

/**
 * Calculates the base amount of experience gained for a skill,
 * excluding growth buffs.
 *
 * @param base_experience The base experience coefficient
 * @param potential The skill's potential
 * @param skills().level The skill's level
 *
 * @return The adjusted experience amount
 */
int calc_base_skill_exp_gained(
    int base_experience,
    int potential,
    int skill_level);

/**
 * Calculates the amount of experience gained based on an attibute growth buff.
 *
 * @param experience The base experience level
 * @param buff_amount The buff amount
 *
 * @return The adjusted experience amount
 */
int calc_boosted_skill_exp_gained(int experience, int buff_amount);

/**
 * Calculates the amount of character experience gained from gaining
 * skill experience.
 *
 * @param chara The character
 * @param skill_exp The gained amount of experience
 * @param divisor The experience divisor of character level
 *
 * @return The adjusted experience amount
 */
int calc_chara_exp_from_skill_exp(
    const Character& chara,
    int skill_exp,
    int divisor);

int calc_exp_gain_negotiation_gold_threshold(int current_level);
int calc_exp_gain_negotiation(int gold, int current_level);
int calc_exp_gain_detection(int dungeon_level);
int calc_spell_exp_gain(int spell_id);
int calc_exp_gain_casting(int spell_id);
int calc_exp_gain_mana_capacity(const Character& chara);
int calc_exp_gain_healing(const Character& chara);
int calc_exp_gain_meditation(const Character& chara);
int calc_exp_gain_stealth();
int calc_exp_gain_weight_lifting(const Character& chara);
int calc_exp_gain_memorization(int spell_id);
int calc_exp_gain_crafting(int mat_amount);

} // namespace elona
