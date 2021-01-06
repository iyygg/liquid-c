#include <ruby.h>
#include "liquid.h"
#include "document.h"
#include "document_body.h"
#include "serialize_parse_context.h"
#include "tokenizer.h"

static ID id_ivar_root, id_configure_options;

static VALUE marshal_load_constants(const char *str, size_t len)
{
    VALUE str_obj = rb_str_new_static(str, len);
    VALUE constants = rb_marshal_load(str_obj);
    if (BUILTIN_TYPE(constants) != T_ARRAY) {
        rb_raise(rb_eArgError, "expected constants to be an array");
    }
    return constants;
}

static VALUE template_load(VALUE self, VALUE source, VALUE options)
{
    rb_funcall(self, id_configure_options, 1, options);

    Check_Type(source, T_STRING);
    source = rb_str_dup_frozen(source);
    const char *data = RSTRING_PTR(source);

    document_body_header_t *header = (document_body_header_t *)data;

    assert(RSTRING_LEN(source) >= header->buffer_offset + header->buffer_offset);
    const char *body_data = data + header->buffer_offset;

    assert(RSTRING_LEN(source) >= header->constants_offset + header->constants_len);
    VALUE constants = marshal_load_constants(data + header->constants_offset, header->constants_len);

    VALUE document_body = document_body_new_immutable_instance(constants, source, body_data);

    VALUE parse_context = serialize_parse_context_new(document_body, header);

    rb_ivar_set(self, id_ivar_root, document_parse(Qnil, parse_context));

    return self;
}

void liquid_define_template()
{
    id_ivar_root = rb_intern("@root");
    id_configure_options = rb_intern("configure_options");

    VALUE cLiquidTemplate = rb_const_get(mLiquid, rb_intern("Template"));
    rb_define_method(cLiquidTemplate, "load", template_load, 2);
}