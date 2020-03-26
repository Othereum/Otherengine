#pragma once
#include <memory>
#include <unordered_map>
#include "ActorComponent.h"
#include "Name.h"

namespace game
{
	namespace ai_state
	{
		class Base;
	}
	
	class AIComponent : public CActorComponent
	{
	public:
		AIComponent(AActor& owner, int update_order = 100);
		void Update(float delta_seconds) override;
		
		/**
		 * \brief Change state. You must add state before change, or std::out_of_bound exception will be thrown
		 * \param name New state name
		 */
		void ChangeState(FName name);
		
		void AddState(std::unique_ptr<ai_state::Base>&& state);

		template <class T, class... Args>
		void AddState(Args&&... args)
		{
			AddState(std::make_unique<T>(std::forward<Args>(args)...));
		}

	private:
		std::unordered_map<FName, std::unique_ptr<ai_state::Base>> states_;
		std::reference_wrapper<ai_state::Base> cur_;
	};
}
