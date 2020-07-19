#pragma once
#include "Name.hpp"
#include "Components/ActorComponent.hpp"
#include "Templates/HashMap.hpp"

namespace oeng::engine
{
	namespace ai_state
	{
		class Base;
	}
	
	class OEAPI AIComponent : public ActorComponent
	{
	public:
		AIComponent(AActor& owner, int update_order = 100);
		
		/**
		 * Change state. You must add state before change, or std::out_of_bound exception will be thrown
		 * @param name New state name
		 */
		void ChangeState(Name name);
		
		void AddState(UniquePtr<ai_state::Base>&& state);

		template <class T, class... Args>
		void AddState(Args&&... args)
		{
			AddState(MakeUnique<T>(std::forward<Args>(args)...));
		}

	private:
		void OnUpdate(Float delta_seconds) override;
		
		HashMap<Name, UniquePtr<ai_state::Base>> states_;
		std::reference_wrapper<ai_state::Base> cur_;
	};
}
