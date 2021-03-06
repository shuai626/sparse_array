#include "sparse_array.h"

/* Creates an empty sparse array of length size 
  (the size of the underlying bitvector you will create). */
void sparse_array::create(uint64_t size) {
  sdsl::bit_vector b(size);
  b_ = b;
}

/* Appends the element elem at index pos of the sparse array. 
  You may assume this is the only way to insert elements, and 
  so you will always insert element in-order and never will 
  insert two element with the same position, further you may 
  assume that you will always have pos < size (but you should 
  probably guard against this anyway). */
void sparse_array::append(string elem, uint64_t pos) {
  if (pos <= b_.size()) {
    b_[pos - 1] = 1;

    elems_.push_back(elem);
  }
}

/* This function places a reference to the r-th present item in the 
  array in the reference elem. It returns true if there was >= r items
 in the sparse array and false otherwise.*/
bool sparse_array::get_at_rank(uint64_t r, string& elem) {
  if (r <= elems_.size()) {
    elem = elems_[r-1];
    return true;
  } 
  else {
    return false;
  }
}

/* This function looks at the r-th index in the sparse bitvector;
 if that bit is 1, it fetches the corresponding value and binds it 
 to the reference elem and returns true, if that bit is a 0, it simply 
 returns false. */
bool sparse_array::get_at_index(uint64_t r, string& elem) {
  if (b_[r-1] == 1) {
    rank_support r_(&b_);
    uint64_t rank = r_.rank1(r);
    elem = elems_[rank-1];

    return true;
  }
  else {
    return false;
  }
}

/* This function returns the count of present elements 
  (1s in the bit vector) up to and including index r. */
uint64_t sparse_array::num_elem_at(uint64_t r) {
  rank_support r_(&b_);
  return r_.rank1(r);
}

/*  Returns the size of the sparse array. */
uint64_t sparse_array::size() {
  return b_.size();
}

/* Returns the number of present elements in the sparse array 
  (i.e. the number of 1s in the bitvector). */
uint64_t sparse_array::num_elem() {
  rank_support r_(&b_);
  return r_.rank1(b_.size());
}

/* Saves the sparse array to the file fname.*/
void sparse_array::save(string& fname) {
  ofstream out;
  out.open(fname);
  string bv_fname = fname + "_bv";
  sdsl::store_to_file(b_, bv_fname);
  out << bv_fname << endl;

  int i;
  for (i = 0; i < elems_.size(); i++) {
    string s = elems_.at(i);
    out << s << endl;
  }
  
  out.close();

  return;
}

/* Loads the sparse array from the file fname.*/
void sparse_array::load(string& fname) {
  ifstream in(fname);
  string line;

  getline(in, line);
  sdsl::load_vector_from_file(b_, line, 0);

  elems_.clear();
  
  while(getline(in, line))
	{
		elems_.push_back(line);
	}

  in.close();

  return;
}