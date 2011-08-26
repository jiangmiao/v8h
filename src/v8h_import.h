#ifndef V8H_V8H_IMPORT_H
#define V8H_V8H_IMPORT_H

// DECALE
#define V8H_DECLARE_NEW_INSTANCE() \
	Handle<Object> new_instance();

#define V8H_DECLARE_DESTRUCTOR() \
        void destructor(Persistent<Value> value, void *data);

#define V8H_DECLARE_GET_INTERNAL() \
	InternalPointer get_internal(Handle<Object> self);

#define V8H_DECLARE_UNWRAP() \
	InternalPointer Unwrap(Handle<Value> value);
	
#define V8H_DECLARE_GLOBAL() \
	Persistent<Object> global();

// IMPORT
#define V8H_IMPORT_GLOBAL() \
	Persistent<Object> global() \
	{ \
		static auto name =  Persistent<Object>::New(new_instance()); \
		return name; \
	}
#define V8H_IMPORT_NEW_INSTANCE() \
	Handle<Object> new_instance() \
	{ \
		return create()->NewInstance(); \
	}

#define V8H_IMPORT_DESTRUCTOR_FUNCTION() \
        void destructor(Persistent<Value> value, void *data) { \
		DEL_PTR<InternalPointer>(value->ToObject(), FIELD_INTERNAL); \
		puts(__FUNCTION__); \
	}

#define V8H_IMPORT_DESTRUCTOR() \
        void destructor(Persistent<Value> value, void *data) { \
		DEL_PTR<InternalPointer>(value->ToObject(), FIELD_INTERNAL); \
		puts(__FUNCTION__); \
	}
#define V8H_IMPORT_GET_INTERNAL() \
	InternalPointer get_internal(Handle<Object> self) \
	{ \
		return (InternalPointer)GET_PTR(self, FIELD_INTERNAL); \
	}

#define V8H_IMPORT_UNWRAP() \
	InternalPointer Unwrap(Handle<Value> value) \
	{ \
		return (FILE*)External::Unwrap(value); \
	}
	

#define V8H_IMPORT_GLOBAL() \
	Persistent<Object> global() \
	{ \
		static auto name =  Persistent<Object>::New(new_instance()); \
		return name; \
	}

// IMPORT WITH CLASS
#define V8H_IMPORT_GET_INTERNAL_WITH_CLASS(klass) \
	klass::InternalPointer klass::get_internal(Handle<Object> self) \
	{ \
		return (InternalPointer)GET_PTR(self, FIELD_INTERNAL); \
	}

#define V8H_IMPORT_UNWRAP_WITH_CLASS(klass) \
	klass::InternalPointer klass::Unwrap(Handle<Value> value) \
	{ \
		return (FILE*)External::Unwrap(value); \
	}
	

#define V8H_IMPORT_GLOBAL_WITH_CLASS(klass) \
	Persistent<Object> klass::global() \
	{ \
		static auto name =  Persistent<Object>::New(new_instance()); \
		return name; \
	}

#define V8H_IMPORT_NEW_INSTANCE_WITH_CLASS(klass) \
	Handle<Object> klass::new_instance() \
	{ \
		return create()->NewInstance(); \
	}

#define V8H_IMPORT_DESTRUCTOR_WITH_CLASS(klass) \
        void klass::destructor(Persistent<Value> value, void *data) { \
		DEL_PTR<InternalPointer>(value->ToObject(), FIELD_INTERNAL); \
		puts(__FUNCTION__); \
	}
#endif
