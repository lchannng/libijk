// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: rpc_meta.proto

#ifndef PROTOBUF_INCLUDED_rpc_5fmeta_2eproto
#define PROTOBUF_INCLUDED_rpc_5fmeta_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/map.h>  // IWYU pragma: export
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_rpc_5fmeta_2eproto 

namespace protobuf_rpc_5fmeta_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[4];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_rpc_5fmeta_2eproto
namespace ijk {
class RpcMeta;
class RpcMetaDefaultTypeInternal;
extern RpcMetaDefaultTypeInternal _RpcMeta_default_instance_;
class RpcMeta_MetaDataEntry_DoNotUse;
class RpcMeta_MetaDataEntry_DoNotUseDefaultTypeInternal;
extern RpcMeta_MetaDataEntry_DoNotUseDefaultTypeInternal _RpcMeta_MetaDataEntry_DoNotUse_default_instance_;
class RpcMeta_Request;
class RpcMeta_RequestDefaultTypeInternal;
extern RpcMeta_RequestDefaultTypeInternal _RpcMeta_Request_default_instance_;
class RpcMeta_Response;
class RpcMeta_ResponseDefaultTypeInternal;
extern RpcMeta_ResponseDefaultTypeInternal _RpcMeta_Response_default_instance_;
}  // namespace ijk
namespace google {
namespace protobuf {
template<> ::ijk::RpcMeta* Arena::CreateMaybeMessage<::ijk::RpcMeta>(Arena*);
template<> ::ijk::RpcMeta_MetaDataEntry_DoNotUse* Arena::CreateMaybeMessage<::ijk::RpcMeta_MetaDataEntry_DoNotUse>(Arena*);
template<> ::ijk::RpcMeta_Request* Arena::CreateMaybeMessage<::ijk::RpcMeta_Request>(Arena*);
template<> ::ijk::RpcMeta_Response* Arena::CreateMaybeMessage<::ijk::RpcMeta_Response>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace ijk {

enum RpcMeta_Type {
  RpcMeta_Type_REQUEST = 0,
  RpcMeta_Type_RESPONSE = 1,
  RpcMeta_Type_RpcMeta_Type_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  RpcMeta_Type_RpcMeta_Type_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool RpcMeta_Type_IsValid(int value);
const RpcMeta_Type RpcMeta_Type_Type_MIN = RpcMeta_Type_REQUEST;
const RpcMeta_Type RpcMeta_Type_Type_MAX = RpcMeta_Type_RESPONSE;
const int RpcMeta_Type_Type_ARRAYSIZE = RpcMeta_Type_Type_MAX + 1;

const ::google::protobuf::EnumDescriptor* RpcMeta_Type_descriptor();
inline const ::std::string& RpcMeta_Type_Name(RpcMeta_Type value) {
  return ::google::protobuf::internal::NameOfEnum(
    RpcMeta_Type_descriptor(), value);
}
inline bool RpcMeta_Type_Parse(
    const ::std::string& name, RpcMeta_Type* value) {
  return ::google::protobuf::internal::ParseNamedEnum<RpcMeta_Type>(
    RpcMeta_Type_descriptor(), name, value);
}
enum RpcMeta_DataEncodingType {
  RpcMeta_DataEncodingType_BINARY = 0,
  RpcMeta_DataEncodingType_JSON = 1,
  RpcMeta_DataEncodingType_RpcMeta_DataEncodingType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  RpcMeta_DataEncodingType_RpcMeta_DataEncodingType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool RpcMeta_DataEncodingType_IsValid(int value);
const RpcMeta_DataEncodingType RpcMeta_DataEncodingType_DataEncodingType_MIN = RpcMeta_DataEncodingType_BINARY;
const RpcMeta_DataEncodingType RpcMeta_DataEncodingType_DataEncodingType_MAX = RpcMeta_DataEncodingType_JSON;
const int RpcMeta_DataEncodingType_DataEncodingType_ARRAYSIZE = RpcMeta_DataEncodingType_DataEncodingType_MAX + 1;

const ::google::protobuf::EnumDescriptor* RpcMeta_DataEncodingType_descriptor();
inline const ::std::string& RpcMeta_DataEncodingType_Name(RpcMeta_DataEncodingType value) {
  return ::google::protobuf::internal::NameOfEnum(
    RpcMeta_DataEncodingType_descriptor(), value);
}
inline bool RpcMeta_DataEncodingType_Parse(
    const ::std::string& name, RpcMeta_DataEncodingType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<RpcMeta_DataEncodingType>(
    RpcMeta_DataEncodingType_descriptor(), name, value);
}
// ===================================================================

class RpcMeta_Request : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ijk.RpcMeta.Request) */ {
 public:
  RpcMeta_Request();
  virtual ~RpcMeta_Request();

  RpcMeta_Request(const RpcMeta_Request& from);

  inline RpcMeta_Request& operator=(const RpcMeta_Request& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  RpcMeta_Request(RpcMeta_Request&& from) noexcept
    : RpcMeta_Request() {
    *this = ::std::move(from);
  }

  inline RpcMeta_Request& operator=(RpcMeta_Request&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const RpcMeta_Request& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const RpcMeta_Request* internal_default_instance() {
    return reinterpret_cast<const RpcMeta_Request*>(
               &_RpcMeta_Request_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(RpcMeta_Request* other);
  friend void swap(RpcMeta_Request& a, RpcMeta_Request& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline RpcMeta_Request* New() const final {
    return CreateMaybeMessage<RpcMeta_Request>(NULL);
  }

  RpcMeta_Request* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<RpcMeta_Request>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const RpcMeta_Request& from);
  void MergeFrom(const RpcMeta_Request& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RpcMeta_Request* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string method_name = 2;
  void clear_method_name();
  static const int kMethodNameFieldNumber = 2;
  const ::std::string& method_name() const;
  void set_method_name(const ::std::string& value);
  #if LANG_CXX11
  void set_method_name(::std::string&& value);
  #endif
  void set_method_name(const char* value);
  void set_method_name(const char* value, size_t size);
  ::std::string* mutable_method_name();
  ::std::string* release_method_name();
  void set_allocated_method_name(::std::string* method_name);

  // uint64 method_id = 1;
  void clear_method_id();
  static const int kMethodIdFieldNumber = 1;
  ::google::protobuf::uint64 method_id() const;
  void set_method_id(::google::protobuf::uint64 value);

  // uint64 sequence_id = 4;
  void clear_sequence_id();
  static const int kSequenceIdFieldNumber = 4;
  ::google::protobuf::uint64 sequence_id() const;
  void set_sequence_id(::google::protobuf::uint64 value);

  // uint64 timeout = 5;
  void clear_timeout();
  static const int kTimeoutFieldNumber = 5;
  ::google::protobuf::uint64 timeout() const;
  void set_timeout(::google::protobuf::uint64 value);

  // bool expect_response = 3;
  void clear_expect_response();
  static const int kExpectResponseFieldNumber = 3;
  bool expect_response() const;
  void set_expect_response(bool value);

  // @@protoc_insertion_point(class_scope:ijk.RpcMeta.Request)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr method_name_;
  ::google::protobuf::uint64 method_id_;
  ::google::protobuf::uint64 sequence_id_;
  ::google::protobuf::uint64 timeout_;
  bool expect_response_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_rpc_5fmeta_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class RpcMeta_Response : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ijk.RpcMeta.Response) */ {
 public:
  RpcMeta_Response();
  virtual ~RpcMeta_Response();

  RpcMeta_Response(const RpcMeta_Response& from);

  inline RpcMeta_Response& operator=(const RpcMeta_Response& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  RpcMeta_Response(RpcMeta_Response&& from) noexcept
    : RpcMeta_Response() {
    *this = ::std::move(from);
  }

  inline RpcMeta_Response& operator=(RpcMeta_Response&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const RpcMeta_Response& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const RpcMeta_Response* internal_default_instance() {
    return reinterpret_cast<const RpcMeta_Response*>(
               &_RpcMeta_Response_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(RpcMeta_Response* other);
  friend void swap(RpcMeta_Response& a, RpcMeta_Response& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline RpcMeta_Response* New() const final {
    return CreateMaybeMessage<RpcMeta_Response>(NULL);
  }

  RpcMeta_Response* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<RpcMeta_Response>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const RpcMeta_Response& from);
  void MergeFrom(const RpcMeta_Response& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RpcMeta_Response* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string reason = 4;
  void clear_reason();
  static const int kReasonFieldNumber = 4;
  const ::std::string& reason() const;
  void set_reason(const ::std::string& value);
  #if LANG_CXX11
  void set_reason(::std::string&& value);
  #endif
  void set_reason(const char* value);
  void set_reason(const char* value, size_t size);
  ::std::string* mutable_reason();
  ::std::string* release_reason();
  void set_allocated_reason(::std::string* reason);

  // uint64 sequence_id = 1;
  void clear_sequence_id();
  static const int kSequenceIdFieldNumber = 1;
  ::google::protobuf::uint64 sequence_id() const;
  void set_sequence_id(::google::protobuf::uint64 value);

  // int32 error_code = 3;
  void clear_error_code();
  static const int kErrorCodeFieldNumber = 3;
  ::google::protobuf::int32 error_code() const;
  void set_error_code(::google::protobuf::int32 value);

  // bool failed = 2;
  void clear_failed();
  static const int kFailedFieldNumber = 2;
  bool failed() const;
  void set_failed(bool value);

  // bool timeout = 5;
  void clear_timeout();
  static const int kTimeoutFieldNumber = 5;
  bool timeout() const;
  void set_timeout(bool value);

  // @@protoc_insertion_point(class_scope:ijk.RpcMeta.Response)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr reason_;
  ::google::protobuf::uint64 sequence_id_;
  ::google::protobuf::int32 error_code_;
  bool failed_;
  bool timeout_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_rpc_5fmeta_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class RpcMeta_MetaDataEntry_DoNotUse : public ::google::protobuf::internal::MapEntry<RpcMeta_MetaDataEntry_DoNotUse, 
    ::std::string, ::std::string,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    0 > {
public:
  typedef ::google::protobuf::internal::MapEntry<RpcMeta_MetaDataEntry_DoNotUse, 
    ::std::string, ::std::string,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
    0 > SuperType;
  RpcMeta_MetaDataEntry_DoNotUse();
  RpcMeta_MetaDataEntry_DoNotUse(::google::protobuf::Arena* arena);
  void MergeFrom(const RpcMeta_MetaDataEntry_DoNotUse& other);
  static const RpcMeta_MetaDataEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const RpcMeta_MetaDataEntry_DoNotUse*>(&_RpcMeta_MetaDataEntry_DoNotUse_default_instance_); }
  void MergeFrom(const ::google::protobuf::Message& other) final;
  ::google::protobuf::Metadata GetMetadata() const;
};

// -------------------------------------------------------------------

class RpcMeta : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ijk.RpcMeta) */ {
 public:
  RpcMeta();
  virtual ~RpcMeta();

  RpcMeta(const RpcMeta& from);

  inline RpcMeta& operator=(const RpcMeta& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  RpcMeta(RpcMeta&& from) noexcept
    : RpcMeta() {
    *this = ::std::move(from);
  }

  inline RpcMeta& operator=(RpcMeta&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const RpcMeta& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const RpcMeta* internal_default_instance() {
    return reinterpret_cast<const RpcMeta*>(
               &_RpcMeta_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  void Swap(RpcMeta* other);
  friend void swap(RpcMeta& a, RpcMeta& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline RpcMeta* New() const final {
    return CreateMaybeMessage<RpcMeta>(NULL);
  }

  RpcMeta* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<RpcMeta>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const RpcMeta& from);
  void MergeFrom(const RpcMeta& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RpcMeta* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef RpcMeta_Request Request;
  typedef RpcMeta_Response Response;

  typedef RpcMeta_Type Type;
  static const Type REQUEST =
    RpcMeta_Type_REQUEST;
  static const Type RESPONSE =
    RpcMeta_Type_RESPONSE;
  static inline bool Type_IsValid(int value) {
    return RpcMeta_Type_IsValid(value);
  }
  static const Type Type_MIN =
    RpcMeta_Type_Type_MIN;
  static const Type Type_MAX =
    RpcMeta_Type_Type_MAX;
  static const int Type_ARRAYSIZE =
    RpcMeta_Type_Type_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Type_descriptor() {
    return RpcMeta_Type_descriptor();
  }
  static inline const ::std::string& Type_Name(Type value) {
    return RpcMeta_Type_Name(value);
  }
  static inline bool Type_Parse(const ::std::string& name,
      Type* value) {
    return RpcMeta_Type_Parse(name, value);
  }

  typedef RpcMeta_DataEncodingType DataEncodingType;
  static const DataEncodingType BINARY =
    RpcMeta_DataEncodingType_BINARY;
  static const DataEncodingType JSON =
    RpcMeta_DataEncodingType_JSON;
  static inline bool DataEncodingType_IsValid(int value) {
    return RpcMeta_DataEncodingType_IsValid(value);
  }
  static const DataEncodingType DataEncodingType_MIN =
    RpcMeta_DataEncodingType_DataEncodingType_MIN;
  static const DataEncodingType DataEncodingType_MAX =
    RpcMeta_DataEncodingType_DataEncodingType_MAX;
  static const int DataEncodingType_ARRAYSIZE =
    RpcMeta_DataEncodingType_DataEncodingType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  DataEncodingType_descriptor() {
    return RpcMeta_DataEncodingType_descriptor();
  }
  static inline const ::std::string& DataEncodingType_Name(DataEncodingType value) {
    return RpcMeta_DataEncodingType_Name(value);
  }
  static inline bool DataEncodingType_Parse(const ::std::string& name,
      DataEncodingType* value) {
    return RpcMeta_DataEncodingType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // map<string, string> meta_data = 5;
  int meta_data_size() const;
  void clear_meta_data();
  static const int kMetaDataFieldNumber = 5;
  const ::google::protobuf::Map< ::std::string, ::std::string >&
      meta_data() const;
  ::google::protobuf::Map< ::std::string, ::std::string >*
      mutable_meta_data();

  // .ijk.RpcMeta.Request request_info = 3;
  bool has_request_info() const;
  void clear_request_info();
  static const int kRequestInfoFieldNumber = 3;
  private:
  const ::ijk::RpcMeta_Request& _internal_request_info() const;
  public:
  const ::ijk::RpcMeta_Request& request_info() const;
  ::ijk::RpcMeta_Request* release_request_info();
  ::ijk::RpcMeta_Request* mutable_request_info();
  void set_allocated_request_info(::ijk::RpcMeta_Request* request_info);

  // .ijk.RpcMeta.Response response_info = 4;
  bool has_response_info() const;
  void clear_response_info();
  static const int kResponseInfoFieldNumber = 4;
  private:
  const ::ijk::RpcMeta_Response& _internal_response_info() const;
  public:
  const ::ijk::RpcMeta_Response& response_info() const;
  ::ijk::RpcMeta_Response* release_response_info();
  ::ijk::RpcMeta_Response* mutable_response_info();
  void set_allocated_response_info(::ijk::RpcMeta_Response* response_info);

  // .ijk.RpcMeta.Type type = 1;
  void clear_type();
  static const int kTypeFieldNumber = 1;
  ::ijk::RpcMeta_Type type() const;
  void set_type(::ijk::RpcMeta_Type value);

  // uint32 service_id = 2;
  void clear_service_id();
  static const int kServiceIdFieldNumber = 2;
  ::google::protobuf::uint32 service_id() const;
  void set_service_id(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:ijk.RpcMeta)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::MapField<
      RpcMeta_MetaDataEntry_DoNotUse,
      ::std::string, ::std::string,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      ::google::protobuf::internal::WireFormatLite::TYPE_STRING,
      0 > meta_data_;
  ::ijk::RpcMeta_Request* request_info_;
  ::ijk::RpcMeta_Response* response_info_;
  int type_;
  ::google::protobuf::uint32 service_id_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_rpc_5fmeta_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RpcMeta_Request

// uint64 method_id = 1;
inline void RpcMeta_Request::clear_method_id() {
  method_id_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 RpcMeta_Request::method_id() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Request.method_id)
  return method_id_;
}
inline void RpcMeta_Request::set_method_id(::google::protobuf::uint64 value) {
  
  method_id_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Request.method_id)
}

// string method_name = 2;
inline void RpcMeta_Request::clear_method_name() {
  method_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& RpcMeta_Request::method_name() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Request.method_name)
  return method_name_.GetNoArena();
}
inline void RpcMeta_Request::set_method_name(const ::std::string& value) {
  
  method_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Request.method_name)
}
#if LANG_CXX11
inline void RpcMeta_Request::set_method_name(::std::string&& value) {
  
  method_name_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:ijk.RpcMeta.Request.method_name)
}
#endif
inline void RpcMeta_Request::set_method_name(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  method_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:ijk.RpcMeta.Request.method_name)
}
inline void RpcMeta_Request::set_method_name(const char* value, size_t size) {
  
  method_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:ijk.RpcMeta.Request.method_name)
}
inline ::std::string* RpcMeta_Request::mutable_method_name() {
  
  // @@protoc_insertion_point(field_mutable:ijk.RpcMeta.Request.method_name)
  return method_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* RpcMeta_Request::release_method_name() {
  // @@protoc_insertion_point(field_release:ijk.RpcMeta.Request.method_name)
  
  return method_name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void RpcMeta_Request::set_allocated_method_name(::std::string* method_name) {
  if (method_name != NULL) {
    
  } else {
    
  }
  method_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), method_name);
  // @@protoc_insertion_point(field_set_allocated:ijk.RpcMeta.Request.method_name)
}

// bool expect_response = 3;
inline void RpcMeta_Request::clear_expect_response() {
  expect_response_ = false;
}
inline bool RpcMeta_Request::expect_response() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Request.expect_response)
  return expect_response_;
}
inline void RpcMeta_Request::set_expect_response(bool value) {
  
  expect_response_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Request.expect_response)
}

// uint64 sequence_id = 4;
inline void RpcMeta_Request::clear_sequence_id() {
  sequence_id_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 RpcMeta_Request::sequence_id() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Request.sequence_id)
  return sequence_id_;
}
inline void RpcMeta_Request::set_sequence_id(::google::protobuf::uint64 value) {
  
  sequence_id_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Request.sequence_id)
}

// uint64 timeout = 5;
inline void RpcMeta_Request::clear_timeout() {
  timeout_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 RpcMeta_Request::timeout() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Request.timeout)
  return timeout_;
}
inline void RpcMeta_Request::set_timeout(::google::protobuf::uint64 value) {
  
  timeout_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Request.timeout)
}

// -------------------------------------------------------------------

// RpcMeta_Response

// uint64 sequence_id = 1;
inline void RpcMeta_Response::clear_sequence_id() {
  sequence_id_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 RpcMeta_Response::sequence_id() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Response.sequence_id)
  return sequence_id_;
}
inline void RpcMeta_Response::set_sequence_id(::google::protobuf::uint64 value) {
  
  sequence_id_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Response.sequence_id)
}

// bool failed = 2;
inline void RpcMeta_Response::clear_failed() {
  failed_ = false;
}
inline bool RpcMeta_Response::failed() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Response.failed)
  return failed_;
}
inline void RpcMeta_Response::set_failed(bool value) {
  
  failed_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Response.failed)
}

// int32 error_code = 3;
inline void RpcMeta_Response::clear_error_code() {
  error_code_ = 0;
}
inline ::google::protobuf::int32 RpcMeta_Response::error_code() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Response.error_code)
  return error_code_;
}
inline void RpcMeta_Response::set_error_code(::google::protobuf::int32 value) {
  
  error_code_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Response.error_code)
}

// string reason = 4;
inline void RpcMeta_Response::clear_reason() {
  reason_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& RpcMeta_Response::reason() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Response.reason)
  return reason_.GetNoArena();
}
inline void RpcMeta_Response::set_reason(const ::std::string& value) {
  
  reason_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Response.reason)
}
#if LANG_CXX11
inline void RpcMeta_Response::set_reason(::std::string&& value) {
  
  reason_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:ijk.RpcMeta.Response.reason)
}
#endif
inline void RpcMeta_Response::set_reason(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  reason_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:ijk.RpcMeta.Response.reason)
}
inline void RpcMeta_Response::set_reason(const char* value, size_t size) {
  
  reason_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:ijk.RpcMeta.Response.reason)
}
inline ::std::string* RpcMeta_Response::mutable_reason() {
  
  // @@protoc_insertion_point(field_mutable:ijk.RpcMeta.Response.reason)
  return reason_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* RpcMeta_Response::release_reason() {
  // @@protoc_insertion_point(field_release:ijk.RpcMeta.Response.reason)
  
  return reason_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void RpcMeta_Response::set_allocated_reason(::std::string* reason) {
  if (reason != NULL) {
    
  } else {
    
  }
  reason_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), reason);
  // @@protoc_insertion_point(field_set_allocated:ijk.RpcMeta.Response.reason)
}

// bool timeout = 5;
inline void RpcMeta_Response::clear_timeout() {
  timeout_ = false;
}
inline bool RpcMeta_Response::timeout() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.Response.timeout)
  return timeout_;
}
inline void RpcMeta_Response::set_timeout(bool value) {
  
  timeout_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.Response.timeout)
}

// -------------------------------------------------------------------

// -------------------------------------------------------------------

// RpcMeta

// .ijk.RpcMeta.Type type = 1;
inline void RpcMeta::clear_type() {
  type_ = 0;
}
inline ::ijk::RpcMeta_Type RpcMeta::type() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.type)
  return static_cast< ::ijk::RpcMeta_Type >(type_);
}
inline void RpcMeta::set_type(::ijk::RpcMeta_Type value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.type)
}

// uint32 service_id = 2;
inline void RpcMeta::clear_service_id() {
  service_id_ = 0u;
}
inline ::google::protobuf::uint32 RpcMeta::service_id() const {
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.service_id)
  return service_id_;
}
inline void RpcMeta::set_service_id(::google::protobuf::uint32 value) {
  
  service_id_ = value;
  // @@protoc_insertion_point(field_set:ijk.RpcMeta.service_id)
}

// .ijk.RpcMeta.Request request_info = 3;
inline bool RpcMeta::has_request_info() const {
  return this != internal_default_instance() && request_info_ != NULL;
}
inline void RpcMeta::clear_request_info() {
  if (GetArenaNoVirtual() == NULL && request_info_ != NULL) {
    delete request_info_;
  }
  request_info_ = NULL;
}
inline const ::ijk::RpcMeta_Request& RpcMeta::_internal_request_info() const {
  return *request_info_;
}
inline const ::ijk::RpcMeta_Request& RpcMeta::request_info() const {
  const ::ijk::RpcMeta_Request* p = request_info_;
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.request_info)
  return p != NULL ? *p : *reinterpret_cast<const ::ijk::RpcMeta_Request*>(
      &::ijk::_RpcMeta_Request_default_instance_);
}
inline ::ijk::RpcMeta_Request* RpcMeta::release_request_info() {
  // @@protoc_insertion_point(field_release:ijk.RpcMeta.request_info)
  
  ::ijk::RpcMeta_Request* temp = request_info_;
  request_info_ = NULL;
  return temp;
}
inline ::ijk::RpcMeta_Request* RpcMeta::mutable_request_info() {
  
  if (request_info_ == NULL) {
    auto* p = CreateMaybeMessage<::ijk::RpcMeta_Request>(GetArenaNoVirtual());
    request_info_ = p;
  }
  // @@protoc_insertion_point(field_mutable:ijk.RpcMeta.request_info)
  return request_info_;
}
inline void RpcMeta::set_allocated_request_info(::ijk::RpcMeta_Request* request_info) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete request_info_;
  }
  if (request_info) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      request_info = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, request_info, submessage_arena);
    }
    
  } else {
    
  }
  request_info_ = request_info;
  // @@protoc_insertion_point(field_set_allocated:ijk.RpcMeta.request_info)
}

// .ijk.RpcMeta.Response response_info = 4;
inline bool RpcMeta::has_response_info() const {
  return this != internal_default_instance() && response_info_ != NULL;
}
inline void RpcMeta::clear_response_info() {
  if (GetArenaNoVirtual() == NULL && response_info_ != NULL) {
    delete response_info_;
  }
  response_info_ = NULL;
}
inline const ::ijk::RpcMeta_Response& RpcMeta::_internal_response_info() const {
  return *response_info_;
}
inline const ::ijk::RpcMeta_Response& RpcMeta::response_info() const {
  const ::ijk::RpcMeta_Response* p = response_info_;
  // @@protoc_insertion_point(field_get:ijk.RpcMeta.response_info)
  return p != NULL ? *p : *reinterpret_cast<const ::ijk::RpcMeta_Response*>(
      &::ijk::_RpcMeta_Response_default_instance_);
}
inline ::ijk::RpcMeta_Response* RpcMeta::release_response_info() {
  // @@protoc_insertion_point(field_release:ijk.RpcMeta.response_info)
  
  ::ijk::RpcMeta_Response* temp = response_info_;
  response_info_ = NULL;
  return temp;
}
inline ::ijk::RpcMeta_Response* RpcMeta::mutable_response_info() {
  
  if (response_info_ == NULL) {
    auto* p = CreateMaybeMessage<::ijk::RpcMeta_Response>(GetArenaNoVirtual());
    response_info_ = p;
  }
  // @@protoc_insertion_point(field_mutable:ijk.RpcMeta.response_info)
  return response_info_;
}
inline void RpcMeta::set_allocated_response_info(::ijk::RpcMeta_Response* response_info) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete response_info_;
  }
  if (response_info) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      response_info = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, response_info, submessage_arena);
    }
    
  } else {
    
  }
  response_info_ = response_info;
  // @@protoc_insertion_point(field_set_allocated:ijk.RpcMeta.response_info)
}

// map<string, string> meta_data = 5;
inline int RpcMeta::meta_data_size() const {
  return meta_data_.size();
}
inline void RpcMeta::clear_meta_data() {
  meta_data_.Clear();
}
inline const ::google::protobuf::Map< ::std::string, ::std::string >&
RpcMeta::meta_data() const {
  // @@protoc_insertion_point(field_map:ijk.RpcMeta.meta_data)
  return meta_data_.GetMap();
}
inline ::google::protobuf::Map< ::std::string, ::std::string >*
RpcMeta::mutable_meta_data() {
  // @@protoc_insertion_point(field_mutable_map:ijk.RpcMeta.meta_data)
  return meta_data_.MutableMap();
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace ijk

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::ijk::RpcMeta_Type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ijk::RpcMeta_Type>() {
  return ::ijk::RpcMeta_Type_descriptor();
}
template <> struct is_proto_enum< ::ijk::RpcMeta_DataEncodingType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ijk::RpcMeta_DataEncodingType>() {
  return ::ijk::RpcMeta_DataEncodingType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_rpc_5fmeta_2eproto
