namespace hfsm2 {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
ControlT<TA>::Region::Region(ControlT& control_,
							 const RegionID id)
	: control{control_}
	, prevId{control._regionId}
{
	control.setRegion(id);
}

//------------------------------------------------------------------------------

template <typename TA>
ControlT<TA>::Region::~Region() {
	control.resetRegion(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
ControlT<TA>::setRegion(const RegionID id) {
	HFSM_ASSERT(_regionId <= id && id < RegionList::SIZE);

	_regionId = id;
}

//------------------------------------------------------------------------------

template <typename TA>
void
ControlT<TA>::resetRegion(const RegionID id) { //-V524
	HFSM_ASSERT(id <= _regionId && _regionId < RegionList::SIZE);

	_regionId = id;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
PlanControlT<TA>::Origin::Origin(PlanControlT& control_,
								 const StateID id)
	: control{control_}
	, prevId{control._originId}
{
	control.setOrigin(id);
}

//------------------------------------------------------------------------------

template <typename TA>
PlanControlT<TA>::Origin::~Origin() {
	control.resetOrigin(prevId);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
PlanControlT<TA>::Region::Region(PlanControlT& control_,
								 const RegionID id,
								 const StateID index,
								 const LongIndex size)
	: control{control_}
	, prevId{control._regionId}
	, prevIndex{control._regionIndex}
	, prevSize{control._regionSize}
{
	control.setRegion(id, index, size);
}

//------------------------------------------------------------------------------

template <typename TA>
PlanControlT<TA>::Region::~Region() {
	control.resetRegion(prevId, prevIndex, prevSize);

	control._status.clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
PlanControlT<TA>::setOrigin(const StateID id) {
	HFSM_ASSERT(_regionId + _regionSize <= StateList::SIZE);
	HFSM_ASSERT(_originId <= id && id < StateList::SIZE);

	_originId = id;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
PlanControlT<TA>::resetOrigin(const StateID id) { //-V524
	HFSM_ASSERT(_regionId + _regionSize <= StateList::SIZE);
	HFSM_ASSERT(id <= _originId && _originId < StateList::SIZE);

	_originId = id;
}

//------------------------------------------------------------------------------

template <typename TA>
void
PlanControlT<TA>::setRegion(const RegionID id,
							const StateID index,
							const LongIndex size)
{
	HFSM_ASSERT(_regionId <= id && id <  RegionList::SIZE);
	HFSM_ASSERT(_regionIndex <= index && index + size <= _regionIndex + _regionSize);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
PlanControlT<TA>::resetRegion(const RegionID id, //-V524
							  const StateID index,
							  const LongIndex size)
{
	HFSM_ASSERT(id <= _regionId && _regionId < RegionList::SIZE);
	HFSM_ASSERT(index <= _regionIndex && _regionIndex + _regionSize <= index + size);

	_regionId	 = id;
	_regionIndex = index;
	_regionSize	 = size;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
FullControlT<TA>::Lock::Lock(FullControlT& control_)
	: control(!control_._locked ? &control_ : nullptr)
{
	if (control)
		control->_locked = true;
}

//------------------------------------------------------------------------------

template <typename TA>
FullControlT<TA>::Lock::~Lock() {
	if (control)
		control->_locked = false;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
template <typename TState>
Status
FullControlT<TA>::updatePlan(TState& headState,
							 const Status subStatus)
{
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	HFSM_ASSERT(subStatus);

	if (subStatus.result == Status::FAILURE) {
		_status.result = Status::FAILURE;
		headState.wrapPlanFailed(*this);

		if (Plan p = plan(_regionId))
			p.clear();

		return buildPlanStatus<State>();
	} else if (subStatus.result == Status::SUCCESS) {
		if (Plan p = plan(_regionId)) {
			for (auto it = p.first(); it; ++it) {
				if (isActive(it->origin) &&
					_planData.tasksSuccesses.get(it->origin))
				{
					Origin origin{*this, STATE_ID};

					changeTo(it->destination);

					it.remove();
				} else
					break;
			}

			return Status{};
		} else {
			_status.result = Status::SUCCESS;
			headState.wrapPlanSucceeded(*this);

			return buildPlanStatus<State>();
		}
	} else
		return Status{};
}

//------------------------------------------------------------------------------

template <typename TA>
template <typename TState>
Status
FullControlT<TA>::buildPlanStatus() {
	using State = TState;
	static constexpr StateID STATE_ID = State::STATE_ID;

	switch (_status.result) {
	case Status::NONE:
		HFSM_BREAK();
		break;

	case Status::SUCCESS:
		_planData.tasksSuccesses.template set<STATE_ID>();

		HFSM_LOG_PLAN_STATUS(_regionId, StatusEvent::SUCCEEDED);
		break;

	case Status::FAILURE:
		_planData.tasksFailures.template set<STATE_ID>();

		HFSM_LOG_PLAN_STATUS(_regionId, StatusEvent::FAILED);
		break;

	default:
		HFSM_BREAK();
	}

	return {_status.result};
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::changeTo(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::CHANGE, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::CHANGE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::changeTo(const StateID stateId,
						   const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::CHANGE, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::CHANGE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::restart(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RESTART, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RESTART, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::restart(const StateID stateId,
						  const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::RESTART, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RESTART, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::resume(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RESUME, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RESUME, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::resume(const StateID stateId,
						 const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::RESUME, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RESUME, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::utilize(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::UTILIZE, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::UTILIZE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::utilize(const StateID stateId,
						  const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::UTILIZE, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::UTILIZE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::randomize(const StateID stateId) {
	if (!_locked) {
		const Request request{Request::Type::RANDOMIZE, stateId};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RANDOMIZE, stateId);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::randomize(const StateID stateId,
							const Payload& payload)
{
	if (!_locked) {
		const Request request{Request::Type::RANDOMIZE, stateId, payload};
		_requests << request;

		if (_regionIndex + _regionSize <= stateId || stateId < _regionIndex)
			_status.outerTransition = true;

		HFSM_LOG_TRANSITION(_originId, Transition::RANDOMIZE, stateId);
	}
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::schedule(const StateID stateId) {
	const Request transition{Request::Type::SCHEDULE, stateId};
	_requests << transition;

	HFSM_LOG_TRANSITION(_originId, Transition::SCHEDULE, stateId);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::schedule(const StateID stateId,
						   const Payload& payload)
{
	const Request transition{Request::Type::SCHEDULE, stateId, payload};
	_requests << transition;

	HFSM_LOG_TRANSITION(_originId, Transition::SCHEDULE, stateId);
}

//------------------------------------------------------------------------------

template <typename TA>
void
FullControlT<TA>::succeed() {
	_status.result = Status::SUCCESS;

	_planData.tasksSuccesses.set(_originId);

	// TODO: promote taskSuccess all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM_ASSERT(_regionIndex < StateList::SIZE);

		_planData.tasksSuccesses.set(_regionIndex);
	}

	HFSM_LOG_TASK_STATUS(_regionId, _originId, StatusEvent::SUCCEEDED);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename TA>
void
FullControlT<TA>::fail() {
	_status.result = Status::FAILURE;

	_planData.tasksFailures.set(_originId);

	// TODO: promote taskFailure all the way up for all regions without plans
	if (_regionId < RegionList::SIZE && !_planData.planExists.get(_regionId)) {
		HFSM_ASSERT(_regionIndex < StateList::SIZE);

		_planData.tasksFailures.set(_regionIndex);
	}

	HFSM_LOG_TASK_STATUS(_regionId, _originId, StatusEvent::FAILED);
}

////////////////////////////////////////////////////////////////////////////////

template <typename TA>
void
GuardControlT<TA>::cancelPendingTransitions() {
	_cancelled = true;

	HFSM_LOG_CANCELLED_PENDING(_originId);
}

////////////////////////////////////////////////////////////////////////////////

}
}
