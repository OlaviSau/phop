/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_phop.h"


zend_class_entry *;
zend_class_entry *objectmanager_interface_ptr;
zend_class_entry *factory_interface_ptr;
zend_class_entry *config_interface_ptr;


/* {{{ */

ZEND_BEGIN_ARG_INFO_EX(Magento_Framework_ObjectManager_ObjectManager_construct_arginfo, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, factory, factory_interface_ptr, 0)
	ZEND_ARG_OBJ_INFO(0, config, config_interface_ptr, 0)
	ZEND_ARG_TYPE_INFO(1, shared_instances, IS_ARRAY, 1)
ZEND_END_ARG_INFO()


PHP_METHOD(Magento_Framework_ObjectManager_ObjectManager, __construct)
{
	zval *factory;
	zval *config;
	HashTable *shared_instances = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ooh", &factory, &config, &shared_instances) != SUCCESS) {
		return;
	}

	zval *object_manager = getThis();
	
	object_manager->factory = factory;
	object_manager->config = config;

	object_manager->shared_instances = shared_instances;
	object_manager->shared_instances
}
/* }}} */




/* {{{ */
void php_indexed_init(void) {
	zend_object_handlers *zh;
	
	INIT_NS_CLASS_ENTRY(objectmanager_ce, "Magento\\Framework\\ObjectManager", "ObjectManager", ObjectManager_methods);
	Indexed_ce = zend_register_internal_class(&ce);
	Indexed_ce->create_object = php_indexed_create;
	Indexed_ce->get_iterator = php_indexed_iterator;
	Indexed_ce->ce_flags |= ZEND_ACC_FINAL;

	zend_class_implements(
		Indexed_ce, 2,
		spl_ce_ArrayAccess, spl_ce_Countable);

	zh = zend_get_std_object_handlers();	

	memcpy(&php_indexed_handlers, zh, sizeof(zend_object_handlers));
	
	php_indexed_handlers.free_obj = php_indexed_free;
	php_indexed_handlers.get_gc   = php_indexed_gc;
	php_indexed_handlers.get_debug_info = php_indexed_dump;
	php_indexed_handlers.clone_obj = php_indexed_clone;
	php_indexed_handlers.cast_object = php_indexed_cast;

	php_indexed_handlers.read_property = php_indexed_property_read;
	php_indexed_handlers.write_property = php_indexed_property_write;
	php_indexed_handlers.has_property = php_indexed_property_exists;
	php_indexed_handlers.unset_property = php_indexed_property_unset;

	php_indexed_handlers.get_properties = NULL;

	php_indexed_handlers.offset = XtOffsetOf(php_indexed_t, std);
} /* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(phop)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(phop)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(phop)
{
#if defined(COMPILE_DL_PHOP) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(phop)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(phop)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "phop support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ phop_functions[]
 *
 * Every user visible function must have an entry in phop_functions[].
 */
const zend_function_entry phop_functions[] = {
	PHP_FE(confirm_phop_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in phop_functions[] */
};
/* }}} */

/* {{{ phop_module_entry
 */
zend_module_entry phop_module_entry = {
	STANDARD_MODULE_HEADER,
	"phop",
	phop_functions,
	PHP_MINIT(phop),
	PHP_MSHUTDOWN(phop),
	PHP_RINIT(phop),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(phop),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(phop),
	PHP_PHOP_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PHOP
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(phop)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
