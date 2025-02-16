#define HFSM_ENABLE_VERBOSE_DEBUG_LOG
#include "shared.hpp"

namespace test_planner {

//------------------------------------------------------------------------------

using M = hfsm2::Machine;

////////////////////////////////////////////////////////////////////////////////

#define S(s) struct s

using FSM = M::Root<S(Apex),
				M::Composite<S(Planned),
					M::Composite<S(Step1_BT),
						S(Step1_1),
						S(Step1_2),
						S(Step1_3)
					>,
					M::Orthogonal<S(Hybrid),
						M::Composite<S(Step2L_P),
							S(Step2L_1),
							S(Step2L_2)
						>,
						M::Composite<S(Step2R_P),
							S(Step2R_1),
							S(Step2R_2)
						>
					>,
					M::Orthogonal<S(Terminal),
						S(Terminal_L),
						S(Terminal_R)
					>
				>,
				M::Composite<S(Unplanned),
					S(Work_1),
					S(Work_2)
				>
			>;

#undef S

//------------------------------------------------------------------------------

static_assert(FSM::regionId<Apex>()		 ==  0, "");
static_assert(FSM::regionId<Planned>()	 ==  1, "");
static_assert(FSM::regionId<Step1_BT>()	 ==  2, "");
static_assert(FSM::regionId<Hybrid>()	 ==  3, "");
static_assert(FSM::regionId<Step2L_P>()	 ==  4, "");
static_assert(FSM::regionId<Step2R_P>()	 ==  5, "");
static_assert(FSM::regionId<Terminal>()	 ==  6, "");
static_assert(FSM::regionId<Unplanned>() ==  7, "");

static_assert(FSM::stateId<Apex>()		 ==  0, "");
static_assert(FSM::stateId<Planned>()	 ==  1, "");
static_assert(FSM::stateId<Step1_BT>()	 ==  2, "");
static_assert(FSM::stateId<Step1_1>()	 ==  3, "");
static_assert(FSM::stateId<Step1_2>()	 ==  4, "");
static_assert(FSM::stateId<Step1_3>()	 ==  5, "");
static_assert(FSM::stateId<Hybrid>()	 ==  6, "");
static_assert(FSM::stateId<Step2L_P>()	 ==  7, "");
static_assert(FSM::stateId<Step2L_1>()	 ==  8, "");
static_assert(FSM::stateId<Step2L_2>()	 ==  9, "");
static_assert(FSM::stateId<Step2R_P>()	 == 10, "");
static_assert(FSM::stateId<Step2R_1>()	 == 11, "");
static_assert(FSM::stateId<Step2R_2>()	 == 12, "");
static_assert(FSM::stateId<Terminal>()	 == 13, "");
static_assert(FSM::stateId<Terminal_L>() == 14, "");
static_assert(FSM::stateId<Terminal_R>() == 15, "");
static_assert(FSM::stateId<Unplanned>()  == 16, "");
static_assert(FSM::stateId<Work_1>()	 == 17, "");
static_assert(FSM::stateId<Work_2>()	 == 18, "");

////////////////////////////////////////////////////////////////////////////////

struct Apex	  : FSM::State {};

//------------------------------------------------------------------------------

struct Planned
	: FSM::State
{
	void enter(PlanControl& control) {
		auto plan = control.plan();
		REQUIRE(!plan); //-V521

		plan.change<Step1_BT, Hybrid>();
		plan.change<Hybrid, Terminal>();
	}

	void planFailed(FullControl& control) {
		control.succeed();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step1_BT
	: FSM::State
{
	void enter(PlanControl& control) {
		Plan plan = control.plan();
		REQUIRE(!plan); //-V521

		plan.change<Step1_2, Step1_3>();
	}
};

struct Step1_1
	: FSM::State
{
	void update(FullControl& control) {
		control.changeTo<Step1_2>();
	}
};

struct Step1_2
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

struct Step1_3
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct Hybrid
	: FSM::State
{
	void enter(PlanControl& control) {
		auto plan = control.plan();
		REQUIRE(!plan); //-V521

		plan.change<Step2L_1, Step2L_2>();
		plan.change<Step2R_1, Step2R_2>();
	}

	void planFailed(FullControl& control) {
		control.succeed();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2L_P : FSM::State {};

struct Step2L_1
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

struct Step2L_2
	: FSM::State
{
	void update(FullControl& control) {
		control.fail();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Step2R_P : FSM::State {};

struct Step2R_1
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

struct Step2R_2
	: FSM::State
{
	void update(FullControl& control) {
		control.fail();
	}
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

struct Terminal
	: FSM::State
{
	void planSucceeded(FullControl& control) {
		control.fail();
	}
};

struct Terminal_L
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

struct Terminal_R
	: FSM::State
{
	void update(FullControl& control) {
		control.succeed();
	}
};

//------------------------------------------------------------------------------

struct Unplanned  : FSM::State {};
struct Work_1	  : FSM::State {};
struct Work_2	  : FSM::State {};

////////////////////////////////////////////////////////////////////////////////

static_assert(FSM::Instance::STATE_COUNT   == 19, "STATE_COUNT");
static_assert(FSM::Instance::COMPO_REGIONS ==  6, "COMPO_REGIONS");
static_assert(FSM::Instance::COMPO_PRONGS  == 14, "COMPO_PRONGS");
static_assert(FSM::Instance::ORTHO_REGIONS ==  2, "ORTHO_REGIONS");
static_assert(FSM::Instance::ORTHO_UNITS   ==  2, "ORTHO_UNITS");

////////////////////////////////////////////////////////////////////////////////

}
