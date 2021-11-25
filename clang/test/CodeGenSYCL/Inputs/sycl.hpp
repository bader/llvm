#pragma once

inline namespace cl {
namespace sycl {
namespace access {

enum class target {
  global_buffer = 2014,
  constant_buffer,
  local,
  image,
  host_buffer,
  host_image,
  image_array
};

enum class mode {
  read = 1024,
  write,
  read_write,
  discard_write,
  discard_read_write,
  atomic
};

enum class placeholder {
  false_t,
  true_t
};

enum class address_space : int {
  private_space = 0,
  global_space,
  constant_space,
  local_space
};
} // namespace access

template <int dim>
struct id {
  template <typename... T>
  id(T... args) {} // fake constructor
private:
  // Some fake field added to see using of id arguments in the
  // kernel wrapper
  int Data;
};

template <int dim>
struct range {
  template <typename... T>
  range(T... args) {} // fake constructor
private:
  // Some fake field added to see using of range arguments in the
  // kernel wrapper
  int Data;
};

template <int dim>
struct _ImplT {
  range<dim> AccessRange;
  range<dim> MemRange;
  id<dim> Offset;
};

template <typename dataT, int dimensions, access::mode accessmode,
          access::target accessTarget = access::target::global_buffer,
          access::placeholder isPlaceholder = access::placeholder::false_t>
class accessor {

public:
  void use(void) const {}
  template <typename... T>
  void use(T... args) {}
  template <typename... T>
  void use(T... args) const {}
  _ImplT<dimensions> impl;

private:
  void __init(__attribute__((opencl_global)) dataT *Ptr,
              range<dimensions> AccessRange,
              range<dimensions> MemRange, id<dimensions> Offset) {}
};

} // namespace sycl
} // namespace cl
