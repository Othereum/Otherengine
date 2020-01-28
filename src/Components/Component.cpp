#include <Components/Component.h>

NEG_BEGIN

component::component(actor& owner, const int update_order)
	:owner_{owner}, update_order_{update_order}
{
}

NEG_END
