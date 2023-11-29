#include "Action.h"


Application* Application::nullapp = new Application(Effect::nulleff);
Action* Action::Idle = new Action("Idle", { -1, Application::nullapp });

Criteria Action::noCriteria = Criteria();

bool Action::CriteriaPassed(const Status& status)
{
	bool failed = false;
	//looks through all prohibtions, failed = true if there is a effect that be present
	if (criteria.HasProhibitions())
	{
		for (auto& e_c : criteria.GetProhibited())
		{
			failed = status.CheckForEffect(e_c);
			if (failed)
			{
				break;
			}
		}
	}
	if (criteria.HasRequirements() && !failed)
	{
		for (auto& e_c : criteria.GetRequired())
		{
			failed = !status.CheckForEffect(e_c);
			if (failed)
			{
				break;
			}
		}
	}
	return !failed; //return true if nothing triggered a change in the bool
}
